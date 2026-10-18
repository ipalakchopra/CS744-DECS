#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http-parser.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "functions.h"
#include "functionslist.h"
#include "pthread.h"
#include "stdbool.h"
/* 
* Implement all the cflask logic here.
* 1. web server initialization 
* 2. accept http requests
* 3. parse http requests
* 4. dispatch http requests
* 5. process, generate and send http responses
*/

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 64
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t queue_not_full  = PTHREAD_COND_INITIALIZER;

typedef struct{
	int fds[QUEUE_SIZE];
	int front;
	int rear;
	int count;
} Queue;
static Queue q;

void init_queue(Queue *q){
	q->front = 0;
	q->rear = 0;
	q->count = 0;
}

bool empty(Queue *q){
	return (q->count == 0);
}

bool full(Queue *q){
	return (q->count == QUEUE_SIZE);
}

int enqueue(Queue *q, int value){
	if(full(q)){
		perror("Queue full");
		return -1;
	}
	q->fds[q->rear] = value;
	q->rear = (q->rear+1)%QUEUE_SIZE;
	q->count++;
	return 0;
}

int dequeue(Queue *q){
	if(empty(q)){
		perror("Queue empty");
		return 0;
	}
	int value = q->fds[q->front];
	q->front = (q->front+1)%QUEUE_SIZE;
    q->count--;	
	return value;
}

void print_queue(Queue *q) {
    printf("Queue (count=%d): ", q->count);

    for (int i = 0; i < q->count; i++) {
        int index = (q->front + i) % QUEUE_SIZE;
        printf("%d ", q->fds[index]);
    }

    printf("\n");
}
int request_handler(int client_socket, int pid){
    // Request Accept and Parse
    
    char client_request[BUFFER_SIZE];
    ParsedRequest req;
    recv(client_socket, client_request, sizeof(client_request),0);
    
    parse_http_request(client_request, &req);
    
    // Response Creation
    
    int id = get_url_id(req.request_line.uri);
    const char *response;
    
    char *body = NULL;
    
    const char *http_status = NULL;
    
    if (id < 0 || id > 4) {
        response = NULL;
        http_status = "404 Not Found";
        if (asprintf(&body, "{\"status\":\"404 error\"}") == -1) {
            perror("asprintf");
        }
        perror("URL function not found");
		close(client_socket);
		return 1;
    } else {
    	response = p[id]();
    	http_status = "200 OK";
    	if (asprintf(&body,"{\"status\":\"200 OK\",\"message\":\"%s\"}",response ? response : "") == -1) {
    		perror("asprintf");
    	}
    }
    printf("[thread %u] GET %s -> %s\n",pid,req.request_line.uri,http_status);
    
    //printf("%s\n",response);
    
    size_t len = strlen(body);
    size_t response_size = 0;
    
    char *http_response = create_http_response("200 OK", "text/html", body, len, &response_size);
    
    if (http_response == NULL) {
    	perror("create_http_response");
    	return 1;
	}

	ssize_t sent = send(client_socket, http_response, response_size, 0);

	if (sent < 0) {
		perror("send");
	}

	free(http_response);
	close(client_socket);
}

void *request_thread(void *arg){

	printf("in thread");
	int client_socket = *(int *)arg;
	int tid = (unsigned long int)pthread_self();
	request_handler(client_socket, tid);
	return NULL;
}

void *request_pool(){

	while(1){
		pthread_mutex_lock(&mutex);

		while(empty(&q)){
			pthread_cond_wait(&queue_not_empty, &mutex);
		}
		
		int client_fd = dequeue(&q);
		
		pthread_cond_signal(&queue_not_full);
		
		pthread_mutex_unlock(&mutex);
		
		int tid = (unsigned long int)pthread_self();
		request_handler(client_fd,tid);
	}
	return NULL;

}

int main(int argc, char* argv[]) {
    
	int mode = (strcmp(argv[1], "s") == 0) ? 0 :
	           (strcmp(argv[1], "m") == 0) ? 1 :
	           (strcmp(argv[1], "t") == 0) ? 2 : -1;

    int server_fd, client_socket, port=8080;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (argc>2){
		port = atoi(argv[2]);
		if(0>port || port>65535){
			perror("Invalid port number");
			return -1;
		}
    }
	
	// Socket Setup
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("Socket Failure");
		exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
    	perror("Bind Failure");
		exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
    	perror("Listen Failure");
		exit(EXIT_FAILURE);
	}

    printf("cflask [single-threaded] on http://0.0.0.0:%u\n", port);

    // Client Socket
	switch(mode){

		// Single-threaded mode
		case 0:
			while(1){
				if((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
					perror("Accept Failure");
					exit(EXIT_FAILURE);
				}

				int pid = (int)getpid();
				request_handler(client_socket, pid);
			}

		// Multi-threaded mode
		case 1:
			while(1){
				if((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
					perror("Accept Failure");
					exit(EXIT_FAILURE);
				}

				pthread_t thread;

				if(pthread_create(&thread, NULL, &request_thread, &client_socket) != 0){
					perror("Client thread");
					continue;
				}
				pthread_detach(thread);
			}

		// Threadpool mode
		case 2:
			init_queue(&q);
			
			int nthread = atoi(argv[3]);

			for(int i = 0; i<nthread; i++){
				pthread_t thread;
				
				if(pthread_create(&thread, NULL, &request_pool, NULL) != 0){
					perror("Client thread");
					continue;
				}
				pthread_detach(thread);
			}

			while(1){
				if((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
					perror("Accept Failure");
					exit(EXIT_FAILURE);
				}
				pthread_mutex_lock(&mutex);
				
				while(full(&q)){
					pthread_cond_wait(&queue_not_full,&mutex);	
				}

				enqueue(&q,client_socket);
				pthread_cond_signal(&queue_not_empty);
				pthread_mutex_unlock(&mutex);
			}
	}

    return 0;
}

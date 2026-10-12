#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_CHILDREN 100

int main()
{
	pid_t pid;
	int status;
	int reaped = 0;

	printf("I am the Parent process.PID = %d\n", getpid());

	/* Create 100 child processes */
	for (int i = 0; i < NUM_CHILDREN; i++) {
		pid = fork();

		if (pid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (/*TODO : check for child process*/) {

			/* Child process */
            
			srand(time(NULL) ^ getpid());
			int sleep_time = rand() % 40 + 5; // each child sleeps for 5–45 seconds

			printf("I am Child %d with PID = %d\n", i + 1,
			       getpid());
			printf("Sleeping for %d seconds...\n\n", sleep_time);
			fflush(stdout);

			sleep(sleep_time);

			exit((i + 1) % 256);
		}
	}

	printf("Parent created %d child processes.\n\n", NUM_CHILDREN);

	/* Reap children using waitpid() with WNOHANG */
	while ( /*TODO : write condition to check if the parent has child processes*/) 
    {
		int found_child = 0;

		while (/* TODO : check if the child has terminated */) {
			found_child = 1;
			reaped++;

			if (/*TODO : Check if the child exited normally*/) {
				printf("[Parent] Reaped child PID = %d, Exit Status = %d "
				       "(Total Reaped = %d/%d)\n",
				       pid, WEXITSTATUS(status), reaped,
				       NUM_CHILDREN);
			}
		}

		if (!found_child) {
			printf("[Parent] No child has terminated. Sleeping for 5 seconds...\n");
			sleep(5);
		}
	}

	printf("\nAll %d child processes have been reaped.\n", NUM_CHILDREN);
	return 0;
}
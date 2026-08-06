CC=gcc
CCFLAGS=-Wall -g

SRCS=$(wildcard *.c)
OBJ=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
BIN=hu

all: $(BIN)

%.o: %.c
	$(CC) $(CCFLAGS) -MMD -c $< -o $@

-include $(DEPS)

$(BIN): $(OBJ) ../helloworld/helloworld.o
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(BIN) $(DEPS)
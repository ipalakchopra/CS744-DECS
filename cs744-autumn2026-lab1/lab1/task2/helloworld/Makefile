CC=gcc
CCFLAGS=-Wall -g

SRCS=$(wildcard *.c)
OBJ=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
BIN=hw

all: $(BIN)

%.o: %.c
	$(CC) $(CCFLAGS) -MMD -c $< -o $@

-include $(DEPS)

$(BIN): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(BIN) $(DEPS)
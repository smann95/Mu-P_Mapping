CC = gcc

SRCS = $(wildcard *.c)

CFLAGS = -Wall -Wextra -O0 -ggdb -std=c99

LIBS = -lm

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
clean: 
	rm -f $(PROGS)

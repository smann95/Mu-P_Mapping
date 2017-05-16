CC = gcc

CFLAGS = -Wall -Wextra -O0 -ggdb -std=c99

LIBS = -lm

TARGET = fugacity

$(TARGET): main.c fugacity.c 
	$(CC) $(CFLAGS) -o $(TARGET) *.c $(LIBS)
clean:
	rm fugacity

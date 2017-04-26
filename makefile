CC = gcc

CFLAGS = -Wall -Wextra -O2 -ggdb -std=c99

LIBS = -lm
TARGET = fugacity
$(TARGET): main.c fugacity.c 
	$(CC) $(CFLAGS) -o $(TARGET) *.c $(LIBS)
clean:
	rm fugacity

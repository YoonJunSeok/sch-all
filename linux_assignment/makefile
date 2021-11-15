CC=gcc
CFLAGS = -g -O
OBJS=main.o smallsh.o
TARGET=smallsh

$(TARGET): $(OBJS)
	$(CC) $(CFLASGS) -o $(TARGET) $(OBJS)

main.o: smallsh.h main.c
smallsh.o: smallsh.h smallsh.c

clean:
	rm -f *.o smallsh

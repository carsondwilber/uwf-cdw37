CC = gcc
CFLAGS = -Wall
COMPILE = -c
LINK = -g -o

all: virtualDisk

virtualDisk: inode.o fileSystem.o disk.o
	$(CC) $(LIBS) $(CFLAGS) $(LINK) $@ $^

inode.o: inode.c inode.h
	$(CC) $(COMPILE) $<

fileSystem.o: fileSystem.c fileSystem.h
	$(CC) $(COMPILE) $<

disk.o: disk.c disk.h
	$(CC) $(COMPILE) $<

clean:
	rm virtualDisk
	rm *.o

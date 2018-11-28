CC = gcc
FLAGS = -Wall

all:	Simulation

Simulation:	main.o lcs.o io.o
	$(CC) -o $@ $^

main.o:	main.c lcs.h io.h
	$(CC) -c $(FLAGS) -o $@ $<

lcs.o:	lcs.c lcs.h
	$(CC) -c $(FLAGS) -o $@ $<

io.o:	io.c io.h
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -fr COP4534-Proj5 *.o

CC = gcc
CCFLAGS = -Wall

all: mt-collatz

mt-collatz: mt-collatz.o parse.o collatz.o histogram.o
	$(CC) $(CCFLAGS) -g -o $@ $^ -pthread

mt-collatz.o: mt-collatz.c parse.h collatz.h histogram.h
	$(CC) $(CCFLAGS) -c $<

parse.o: parse.c parse.h
	$(CC) $(CCFLAGS) -c $<

collatz.o: collatz.c collatz.h
	$(CC) $(CCFLAGS) -c $<

histogram.o: histogram.c histogram.h
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f mt-collatz *.o

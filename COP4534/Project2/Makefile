all:	Simulation

Simulation:	simulation.o queue.o heap.o customer.o
	gcc -o $@ $^

simulation.o:	main.c queue.h heap.h customer.h
	gcc -c -o $@ $<

queue.o:	queue.c queue.h customer.h
	gcc -c -o $@ $<

heap.o:		heap.c heap.h customer.h
	gcc -c -o $@ $<

customer.o:		customer.c customer.h
	gcc -c -o $@ $<

clean:
	rm -fr COP4534-Proj2 *.o

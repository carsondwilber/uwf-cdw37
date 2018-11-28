all:	Simulation

Simulation:	simulation.o graph.o ga.o util.o timer.o permutation.o
	gcc -o $@ $^

simulation.o:	main.c graph.h ga.h util.h timer.h
	gcc -c -o $@ $<

graph.o:	graph.c graph.h util.h ga.h
	gcc -c -o $@ $<
	
ga.o:	ga.c ga.h util.h permutation.h
	gcc -c -o $@ $<

util.o:	util.c util.h
	gcc -c -o $@ $<

timer.o: 	timer.c timer.h
	gcc -c -o $@ $<

permutation.o: 	permutation.c permutation.h
	gcc -c -o $@ $<

clean:
	rm -fr COP4534-Proj3 *.o

all:	Simulation

Simulation:	main.o dataio.o data.o configuration.o simulation.o
	gcc -o $@ $^ -lm

main.o:	main.c data.h dataio.h configuration.h simulation.h
	gcc -c -o $@ $<

dataio.o:	dataio.c dataio.h
	gcc -c -o $@ $<

data.o:	data.c data.h
	gcc -c -o $@ $<

configuration.o:	configuration.c configuration.h
	gcc -c -o $@ $<

simulation.o:	simulation.c simulation.h configuration.h
	gcc -c -o $@ $<

clean:
	rm -fr COP4534-Proj3 *.o

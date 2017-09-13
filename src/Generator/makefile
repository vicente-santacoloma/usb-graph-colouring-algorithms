CC = gcc
CFLAGS = -Wall -O3 
BINPATH = ${HOME}/color/bin/pc
GEN = main.o utility.o inputs.o printgraph.o whatgraph.o randgraph.o \
	geomgraph.o geomcntrl.o verify.o graph.o cheat.o girth.o flat.o \
	weight.o clique.o cycle.o evacuate.o

generator: $(GEN)
	$(CC) $(CFLAGS) -o generator $(GEN) -lm;\
	echo "To install make install";

install: generator
	mv generator $(BINPATH)/.

graph.o: generate.h graph.h makefile

$(GEN) : generate.h makefile

clean: 
	rm *.o 

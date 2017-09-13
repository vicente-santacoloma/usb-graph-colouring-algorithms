# Environment 
MKDIR=mkdir
CP=cp
OBJECTSGCS1=Grafo.o Nodo.o ColouringAlgorithm.o BrownColouringAlgorithm.o DsaturColouringAlgorithm.o BrownKormanColouringAlgorithm.o BrelazColouringAlgorithm.o
CC=g++
OPTIONS=-Wall -Wextra

# all
all: gcs2

gcs2: $(OBJECTSGCS1) gcs2.cpp
	$(CC) $(OPTIONS) gcs2.cpp $(OBJECTSGCS1) -o $@

%.o: %.cpp %.h
	$(CC) $(OPTIONS) -c $<

clean:
	rm *.o

CC = cc
CFLAGS = -std=c99 -pedantic -Wall
OBJECTS = patch.o

all: patch-core

patch.o: src/patch.c
	$(CC) $(CFLAGS) -c src/patch.c

patch-core: $(OBJECTS)
	$(CC) $(OBJECTS) -o patch-core

clean:
	rm -f *.o patch-core
	Agg 1

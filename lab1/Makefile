CC=gcc
CFLAGS=-I.
DEPS = wrapper.h
OBJ = main.o wrapper.o
LIBS+=-pthread
LIBS+=-lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Lab1: $(OBJ)
	$(CC) -o -g $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o Lab1


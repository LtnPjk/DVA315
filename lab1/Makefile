CC=gcc
CFLAGS=-I.
DEPS = wrapper.h
OBJ = main.o wrapper.o 
LIBS+=-pthread
LIBS+=-lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Lab1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o Lab1


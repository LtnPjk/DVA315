CC=gcc
CFLAGS=-I.
DEPS = wrapper.h
OBJ = main.o 
LIBS+=-pthread
LIBS+=-lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Lab5: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o Lab1


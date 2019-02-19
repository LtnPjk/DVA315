CC=gcc
CFLAGS=-I.
OBJ = main.o
LIBS+=-pthread
LIBS+=-lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

app: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o app


CC=gcc
CFLAGS=-I.

install: build/main.o
	$(CC) -o build/main main.cpp -I.

clean:
	rm -f build/*.o

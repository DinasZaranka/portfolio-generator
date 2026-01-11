CC = gcc

main: src/main.c src/functions.c src/functions.h
	$(CC) src/main.c src/functions.c -o main

clean:
	rm -f main
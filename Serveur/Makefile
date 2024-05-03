all: main.o
	gcc -o prog main.o -Wall -fsanitize=address

main.o: main.c
	gcc -o main.o -c main.c -Wall -fsanitize=address

clean:
	rm *.o
	rm prog

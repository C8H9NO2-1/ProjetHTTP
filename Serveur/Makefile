CFLAGS = -Wall

vpath %.c src

objects = bin/main.o bin/functions.o bin/api.o bin/checkRessource.o
src = src/
bin = bin/
exec = prog
server = server

all : $(exec)

$(exec) : $(objects)
	gcc -o $(exec) $(objects) -fsanitize=address

$(bin)main.o : main.c
	gcc $(CFLAGS) -c $(src)main.c -o $(bin)main.o

$(bin)functions.o : functions.c
	gcc $(CFLAGS) -c $(src)functions.c -o $(bin)functions.o

$(bin)api.o : api.c
	gcc $(CFLAGS) -c $(src)api.c -o $(bin)api.o

$(bin)checkRessource.o : checkRessource.c
	gcc $(CFLAGS) -c $(src)checkRessource.c -o $(bin)checkRessource.o

$(server) : $(objects)
	gcc -fsanitize=address -o server $(objects) -L./$(bin) -lrequest -fsanitize=address
	export LD_LIBRARY_PATH=./$(bin)

clean:
	rm bin/*.o
	rm prog

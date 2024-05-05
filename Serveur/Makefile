CFLAGS = -Wall

vpath %.c src

objects = bin/functions.o bin/api.o bin/pm.o
main1 = bin/main.o
main2 = bin/server.o
src = src/
bin = bin/
exec = prog
server = server

all : $(exec)

$(exec) : $(objects) $(main1)
	gcc -o $(exec) $(objects) $(main1) -fsanitize=address

$(bin)main.o : main.c
	gcc $(CFLAGS) -c $(src)main.c -o $(bin)main.o

$(bin)functions.o : functions.c
	gcc $(CFLAGS) -c $(src)functions.c -o $(bin)functions.o

$(bin)api.o : api.c
	gcc $(CFLAGS) -c $(src)api.c -o $(bin)api.o

$(bin)pm.o : pm.c
	gcc $(CFLAGS) -c $(src)pm.c -o $(bin)pm.o

$(bin)server.o : server.c
	gcc $(CFLAGS) -c $(src)server.c -o $(bin)server.o

$(server) : $(objects) $(main2)
	gcc -fsanitize=address -o server $(objects) $(main2) -L./$(bin) -lrequest -fsanitize=address
	export LD_LIBRARY_PATH=./bin

clean:
	rm bin/*.o
	rm prog
	rm server

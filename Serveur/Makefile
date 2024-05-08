CFLAGS = -Wall

vpath %.c src

src = src/
bin = bin/
exec = prog
server = server
objects = $(addprefix $(bin),functions.o api.o pm.o)
main1 = $(bin)main.o
main2 = $(bin)bin/server.o

all : $(exec)

$(exec) : $(objects) $(main1)
	gcc -o $(exec) $(objects) $(main1) -fsanitize=address

$(bin)%.o : %.c
	gcc $(CFLAGS) -c $< $(OUTPUT_OPTION) 

$(server) : $(objects) $(main2)
	gcc -o server $(objects) $(main2) -L./$(bin) -lrequest -fsanitize=address
	#export LD_LIBRARY_PATH=./bin

.PHONY : clean
clean :
	-rm bin/*.o
	-rm prog
	-rm server

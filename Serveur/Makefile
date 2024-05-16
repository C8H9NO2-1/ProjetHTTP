CFLAGS = -Wall

vpath %.c src

src = src/
bin = bin/
zlib = 1.3.1/
exec = prog
server = server
objects = $(addprefix $(bin),functions.o api.o pm.o elisa.o colours.o)
#objects = $(addprefix $(bin),functions.o api.o pm.o)
main1 = $(bin)main.o
main2 = $(bin)server.o

all : $(exec)

$(exec) : $(objects) $(main1)
	gcc -o $(exec) $(objects) $(main1) -L./$(zlib) -lz -lm -fsanitize=address

$(bin)%.o : %.c
	gcc $(CFLAGS) -c $< $(OUTPUT_OPTION) 

$(server) : $(objects) $(main2)
	gcc -o server $(objects) $(main2) -L./$(bin) -lrequest -L./$(zlib) -lz -lm -fsanitize=address
	#export LD_LIBRARY_PATH=./bin

.PHONY : clean
clean :
	-rm bin/*.o
	-rm prog
	-rm server

CFLAGS = -Wall
UNAME = $(shell uname)

vpath %.c src

src = src/
bin = bin/
ifeq ($(UNAME), Linux)
	zlib = 1.3.1/lib/
endif
ifeq ($(UNAME), Darwin)
	zlib = 1.3.1/
endif
exec = prog
server = server
# objects = $(addprefix $(bin), functions.o)
objects = $(addprefix $(bin),functions.o api.o pm.o ismail.o colours.o response.o tom.o fileLen.o begin.o phpResponse.o FCGI_param.o phpServer.o)
#objects = $(addprefix $(bin),functions.o api.o pm.o colours.o)
main1 = $(bin)main.o
main2 = $(bin)server.o

all : $(exec)

$(exec) : $(objects) $(main1)
	gcc -o $(exec) $(objects) $(main1)  -lm -L./$(zlib) -lz -fsanitize=address
$(bin)%.o : %.c
	gcc $(CFLAGS) -c $< $(OUTPUT_OPTION) 

$(server) : $(objects) $(main2)
	gcc -o server $(objects) $(main2) -L./$(bin) -lrequest -lm -fsanitize=address

	#export LD_LIBRARY_PATH=./bin

.PHONY : clean
clean :
	-rm bin/*.o
	-rm prog
	-rm server

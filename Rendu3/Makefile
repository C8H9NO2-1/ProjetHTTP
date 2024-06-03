CFLAGS = -Wall

vpath %.c src

src = src/
bin = bin/
server = server
objects = $(addprefix $(bin),functions.o api.o pm.o elisa.o ismail.o colours.o response.o tom.o fileLen.o)
#objects = $(addprefix $(bin),functions.o api.o pm.o colours.o)
main = $(bin)server.o

all : $(server)

$(server) : $(objects) $(main)
	gcc -o server $(objects) $(main2) -L./$(bin) -lrequest -lm -fsanitize=address
$(bin)%.o : %.c
	gcc $(CFLAGS) -c $< $(OUTPUT_OPTION) 

.PHONY : clean
clean :
	-rm bin/*.o
	-rm server

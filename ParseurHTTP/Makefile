all: functions.o affichage.o api.o main.o
	gcc -o main functions.o affichage.o main.o api.o -fsanitize=address
	rm *.o

functions.o: functions.c
	gcc -o functions.o -c functions.c

# new.o: newElisa.c
# 	gcc -o new.o -c newElisa.c -fsanitize=address

main.o: main.c
	gcc -o main.o -c main.c

api.o: api.c
	gcc -o api.o -c api.c

affichage.o: affichage.c
	gcc -c affichage.c -o affichage.o

#!===============================================

pm: pm.o affichage.o
	gcc -o prog pm.o affichage.o -fsanitize=address

elisa2: elisa2.o affichage.o
	gcc -o prog elisa2.o affichage.o -fsanitize=address

elisa1: elisa1.o affichage.o
	gcc -o prog elisa1.o affichage.o -fsanitize=address

pm.o: pm.c
	gcc -c pm.c -o pm.o -fsanitize=address

elisa2.o: elisa2.c
	gcc -c elisa2.c -o elisa2.o -fsanitize=address

elisa1.o: elisa1.c
	gcc -c elisa1.c -o elisa1.o -fsanitize=address

tom: tom.o affichage.o
	gcc -o prog tom.o affichage.o -fsanitize=address

tom.o: tom.c
	gcc -c tom.c -o tom.o -fsanitize=address

tom1: tom1.o affichage.o
	gcc -o prog tom1.o affichage.o

tom1.o: tom.c
	gcc -c tom.c -o tom1.o


main: main.o directory.o print.o linkedlist.o
	gcc -o main main.o directory.o print.o linkedlist.o
	rm -f *.o

main.o: main.c
	gcc -c main.c

directory.o: directory.c directory.h
	gcc -c directory.c

print.o: print.c print.h
	gcc -c print.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c

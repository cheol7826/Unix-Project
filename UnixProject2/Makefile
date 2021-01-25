guiserver: Rserver.c guiserver.c
	gcc -o ServerProgram Rserver.c guiserver.c `pkg-config --cflags --libs gtk+-3.0` -pthread
	rm -rf *.o

guiclient: Rclient.c
	gcc -o ClientProgram Rclient.c


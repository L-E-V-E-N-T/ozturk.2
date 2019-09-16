output: main.o
	gcc main.o -o logParse
main.o: main.c
	gcc -c main.c
clean: 
	rm *.o logParse

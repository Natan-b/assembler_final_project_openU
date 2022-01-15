assembler: main.o preprocessor.o
	gcc -g *.c -ansi -Wall -pedantic -o assembler
main.o: main.c compile.h
	gcc -g -c -ansi -Wall -pedantic main.c -o main.o
preprocessor.o: preprocessor.c compile.h
	gcc -g -c -ansi -Wall -pedantic  preprocessor.c -o preprocessor.o

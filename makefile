assembler: main.o preprocessor.o compile.o general_functions.o list.o struct.o hash.o
	gcc *.c -Wall -ansi -pedantic -o assembler
main.o: main.c preprocessor.h
	gcc main.c -Wall -ansi -pedantic -c
preprocessor.o: preprocessor.c preprocessor.h general_functions.h list.h struct.h constants.h compile.h
	gcc preprocessor.c -Wall -ansi -pedantic -c
compile.o: compile.c compile.h general_functions.h list.h struct.h constants.h
	gcc compile.c -Wall -ansi -pedantic -c
general_functions.o: general_functions.c general_functions.h
	gcc general_functions.c -Wall -ansi -pedantic -c
list.o: list.c list.h
	gcc list.c -Wall -ansi -pedantic -c
struct.o: struct.c struct.h constants.h list.h
	gcc struct.c -Wall -ansi -pedantic -c
hash.o: hash.c hash.h constants.h list.h
	gcc hash.c -Wall -ansi -pedantic -c
<<<<<<< HEAD

=======
	
>>>>>>> f7375110181730651ad2cfa6d0b1b039311907c2

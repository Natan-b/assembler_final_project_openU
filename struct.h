#ifndef STRUCT_H
#define STRUCT_H

#include "constants.h"



typedef struct Node_name * ptr_macro_name;

typedef struct Node_name{
	char macro_line[MAX_ONE_LINE];
	ptr_macro_name next;
	}macro_name_node;

<<<<<<< HEAD
/*symbol struct*/
typedef struct Symbol_struct * ptr;

typedef struct Symbol_struct {
	char name[SYMBOL_MAX_LEN];
	/*address of symbol*/
	int value;
	/*base_address of symbol*/
	int base_address;
	/*offset*/
	int offset;
	/*bits array for kind of symbol*/
	int atrributes;
	/*pointer to next node*/
	ptr next;
} symbol_struct;
=======

>>>>>>> f7375110181730651ad2cfa6d0b1b039311907c2

#endif

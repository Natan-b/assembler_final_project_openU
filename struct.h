#ifndef STRUCT_H
#define STRUCT_H

#include "constants.h"




typedef struct Node_name * ptr_macro_name;

typedef struct Node_name{
	char macro_line[MAX_ONE_LINE];
	ptr_macro_name next;
	}macro_name_node;

/*symbol struct*/
typedef struct Symbol_struct * symbol_ptr;
typedef struct Symbol_struct {
	char name[SYMBOL_MAX_LEN];
	/*address of symbol*/
	int value;
	/*base_address of symbol*/
	int base_address;
	/*offset*/
	int offset;
	/*bits array for kind of symbol*/
	int kind;
	/*pointer to next node*/
	symbol_ptr next;
} symbol_struct;

/*create symbol struct function*/
symbol_struct * create_symbol_struct();



#endif

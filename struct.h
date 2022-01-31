#ifndef STRUCT_H
#define STRUCT_H

#include "constants.h"



typedef struct Node_name * ptr_macro_name;

typedef struct Node_name{
	char macro_line[MAX_ONE_LINE];
	ptr_macro_name next;
	}macro_name_node;



#endif

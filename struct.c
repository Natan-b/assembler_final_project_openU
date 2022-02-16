#include "struct.h"
#include "list.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*creating symbol struct*/
symbol_ptr create_symbol_struct()
{
	symbol_struct* cs = malloc(sizeof(symbol_struct));
	cs->name[0] = '\0';
	cs->value = 0;
	cs->base_address = 0;
	cs->offset = 0;
	cs->kind = 0;
	cs->next = NULL;
	return cs;
}


#ifndef HASH_H
#define HASH_H

#include "struct.h"

#define HASHSIZE 100


enum {NOT_FIND,FIND};
enum {NO,YES};

/* index generator for different macro names */
unsigned int hash(char*);
/* the func insert the macro line to the right macro name */
void hash_table_insert(macro_name_node **,char * , char *);
/* the func insert the macro line to the macro linked list  */
void insert_new_line(macro_name_node **,char *);
/* the func free the hash table */
void free_hash_table(macro_name_node **);






#endif

#ifndef HASH_H
#define HASH_H

#define HASHSIZE 100
#include "struct.h"

enum {NOT_FIND,FIND};


void print_table(macro_name_node **);
unsigned int hash(char*);
void init_hash_table();
void hash_table_insert(macro_name_node **,char * , char *);
void insert_new_line(macro_name_node **,char *);
macro_name_node ** creat_hash_table();






#endif

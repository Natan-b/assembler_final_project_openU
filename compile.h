#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_FILE 20
#define MAX_ONE_LINE 80
#define MAX_WORD 25
#define HASHSIZE 100

enum {NOT_FIND,FIND};

typedef struct Node_name * ptr_macro_name;

typedef struct Node_name{
	char macro_line[MAX_ONE_LINE];
	ptr_macro_name next;
	}macro_name_node;




void get_files(int , char **);
void check_file(char *);
int check_line(char *,char*, char *);
void print_table(macro_name_node **);
void clean_line(char *,char *);
int spaceOrTab(char);
unsigned int hash(char*);
void init_hash_table();
void hash_table_insert(macro_name_node **,char * , char *);
void insert_new_line(macro_name_node **,char *);
macro_name_node ** creat_hash_table();



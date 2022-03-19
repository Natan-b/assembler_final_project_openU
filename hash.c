#include "hash.h"
#include "constants.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/* index generator for different macro names */
unsigned int hash(char * macro_name)
{
	int length= strlen(macro_name);
	int i;	
	unsigned int hash_value = 0;
	for(i=0;i<length;i++)
		{
			hash_value+=macro_name[i];
			hash_value = (hash_value * macro_name[i] % HASHSIZE);
		}
	return hash_value;
}

/* the func insert the macro line to the right macro name */
void hash_table_insert(macro_name_node ** hash_table,char * macro_name, char * macro_line)
{
	macro_name_node *head;
	unsigned int hash_value;
	
	hash_value = hash(macro_name);
	
	if(hash_table[hash_value] == NULL)
		{
			hash_table[hash_value] = malloc(sizeof (macro_name_node *));
			
			head = NULL;
			insert_new_line(&head , macro_line);

			hash_table[hash_value]= head;
		}

	else
		{
			head = hash_table[hash_value];
			insert_new_line(&head , macro_line);
		
			hash_table[hash_value]=head;
		}
}

/* the func insert the macro line to the macro linked list  */
void insert_new_line(macro_name_node ** head,char * new_line)
{
	macro_name_node * cur = *head;	
	macro_name_node * new_macro_line = (macro_name_node*)malloc(sizeof(macro_name_node));
	strcpy(new_macro_line->macro_line,new_line);
	new_macro_line->next = NULL;
	if(*head == NULL)
		*head = new_macro_line;

	else 
		{
			while(cur->next !=NULL)
				cur = cur->next;
			cur->next = new_macro_line;
		}

}

/* the func free the hash table */
void free_hash_table(macro_name_node ** hash_table)
{
	if(hash_table!= NULL)
		{
			macro_name_node *head;
			macro_name_node *temp;
			int i;

			for(i=0;i<HASHSIZE;i++)
				{
					head = hash_table[i];
					while(head != NULL)
						{							
							temp = head;
							head = head->next;
							free(temp);
						}
				}
			free(head);		
		}

}

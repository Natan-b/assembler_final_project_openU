#include "struct.h"
#include "list.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void insert_symbol(symbol_struct * cs,char * s_name, int s_value, int atr)
{

	symbol_struct * cur = cs;
	/*creating pointer to new struct*/
	symbol_struct * new;
	/*removing ':' sign from symbol (if there is one)*/
		if(s_name[strlen(s_name)-1] == ':')
			s_name[strlen(s_name)-1] = '\0';
	
	/*if list is a single node*/
	if(cur->next == NULL)
	{
		/*if new symbol name is different from first symbol name*/
		if(strcmp(cur->name,s_name) != 0)
		{
		    /*if symbol list is empty insert parameters into list*/
			if(strlen(cur->name) == 0)
			{
				strcpy(cur->name,s_name);
				cur->value = s_value;
				cur->offset = (s_value%16);
				cur->base_address = (cur->value - cur->offset);
				cur->kind = atr;
				return;
			}
			else
			; /*do nothing*/
			
		}
		else
		{
			printf("\nERROR - symbol already exists in list\n");
			return;	
		}
	}
	
	/*making way to the end of the list while making sure symbol doesn't already exist*/
	while(cur->next != NULL && (strcmp(cur->name,s_name) != 0)) /*CONTINUE HERE*/
	{
		cur = cur->next; /*continue to next node*/
	}
	
	/*if symbol already exists*/
	if(strcmp(cur->name,s_name) == 0)
	{
		printf("\nERROR - symbol already exists in list\n");
		return;
	}
	
	/*creating new node and adding node to the end of list*/
	new = create_symbol_struct();
	strcpy(new->name,s_name);
	new->value = s_value;
	new->offset = (s_value%16);
	new->base_address = (new->value - new->offset);
	new->kind = atr;
	new->next = NULL;
	cur->next = new; /*adding node to symbol list*/
	
}

void print_symbol_list(symbol_struct * cs)
{

	if(cs == NULL)
	printf("\nSymbol list is empty\n");


	while(cs)
	{
		printf("\n%s\n%d\n%d\n%d\n%d\n",cs->name,cs->value,cs->base_address,cs->offset,cs->kind);
		cs = cs->next;
	}


}






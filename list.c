#include "struct.h"
#include "list.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int insert_symbol(symbol_struct * cs,char * s_name, int s_value, int atr)
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
				return 1;
			}
			else
			; /*do nothing*/
			
		}
		else
		{
			if((atr == EXERNAL_SYMBOLKIND) && (cur->kind == EXERNAL_SYMBOLKIND))
				return 1;
			else
				return 0;
				
			
			return 0;	
		}
	}
	
	/*making way to the end of the list while making sure symbol doesn't already exist*/
	while(cur->next != NULL && (strcmp(cur->name,s_name) != 0))
	{
		cur = cur->next; /*continue to next node*/
	}
	
	/*if symbol already exists*/
	if(strcmp(cur->name,s_name) == 0)
	{
		if((atr == EXERNAL_SYMBOLKIND) && (cur->kind == EXERNAL_SYMBOLKIND))
				return 1;
			else
				return 0;
				
		
		return 0;
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
	return 1;
	
}

void insert_command(command_struct * head,char * label,CommandInfo* commandInfo, int arguments_num, int** address,int line_number)
{
	command_struct * cur = head;

	if(cur->next == NULL)
		{ 				
			/*insert data to new node */
			strcpy(cur->label,label);
			cur->commandInfo=commandInfo;
			cur->arguments_num=arguments_num;
			cur->address=(**address);
			cur->line_number=line_number;
			/*creat new node for command list */
			cur->next = create_command_struct();
			
		}
	else{

		while(cur->next!=NULL)
			cur=cur->next;


		/*insert data to new node */
		strcpy(cur->label,label);
		cur->commandInfo=commandInfo;
		cur->arguments_num=arguments_num;
		cur->address=(**address);
		cur->line_number=line_number;


		/* add the new node to the end of the list*/
		cur->next = create_command_struct();
 	}
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

/*debug printing*/
void print_command_list(command_struct * head)
{
	command_struct * cur = head;

	if(cur->next == NULL)
	printf("\nCommand list is empty\n");

	else
		{
			while(cur->next)
			{
				printf("\ncommand->label: %s\ncommand->name: %s\ncommand->arguments_num: %d\ncommand->address: %d\ncommand->line_number: %d\n",cur->label,cur->commandInfo->commandName,cur->arguments_num,cur->address,cur->line_number);
				cur = cur->next;
			}
		}


}

/*debug printing*/
void print_data_list(data_struct * head)
{
	data_struct * cur = head;

	if(cur->next == NULL)
	printf("\nData list is empty\n");

	else
		{
			while(cur)
			{
				printf("\ndata->name: %s\ndata->str_value: %s\ndata->int_values: %d\ndata->int_values_num: %d\ndata->address: %d\n",cur->name,cur->str_value,cur->int_values[0],cur->int_values_num,cur->address);
				cur = cur->next;
			}
		}
}

void free_symbol_list(symbol_struct * head)
{
	if(head!= NULL)
		{			
			symbol_struct *temp;
			while(head != NULL)
				{							
					temp = head;
					head = head->next;
					temp->next=NULL;
					free(temp);
				}
		}
}

void free_command_list(command_struct * head)
{
	if(head!= NULL)
		{			
			command_struct *temp;
			while(head != NULL)
				{							
					temp = head;
					head = head->next;
					temp->next=NULL;
					free(temp);	
				}
		}
}
void free_data_list(data_struct * head)
{
	if(head!= NULL)
		{			
			data_struct *temp;
			while(head != NULL)
				{							
					temp = head;
					head = head->next;
					temp->next=NULL;
					free(temp);
				}
		}
}




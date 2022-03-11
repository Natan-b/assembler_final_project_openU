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


int insert_data(data_struct * data, char * label, char * str, int * values, int values_num, int DC, DataKind kind)
{
		data_struct * cur = data;
		/*creating pointer to new struct*/
		data_struct * new;


		/*if list is a single node*/
		if(cur->next == NULL)
		{
			
		 	   /*if first node of list empty*/
				if((cur->str_value[0]=='\0') && (cur->int_values[0]==0))
				{
					if(kind == 1) /*if is string command*/
					{
						strcpy(cur->name,label);
						strcpy(cur->str_value,str);
						cur->int_values_num = values_num;
						cur->address = DC;
						cur->kind = kind;
						return 1;
					}
					else /*kind == 2*/
					{
						strcpy(cur->name,label);
						memcpy(cur->int_values,values,(sizeof(int)*values_num));
						cur->int_values_num =values_num;
						cur->address = DC;
						cur->kind = kind;
						return 1;
					}
				}
				else
				; /*do nothing*/
		}
		
	
		/*making way to the end of the list*/
		while(cur->next != NULL)
		{
			cur = cur->next; /*continue to next node*/
		}
	
		/*creating new node and adding node to the end of list*/
		new = create_data_struct();
		
		if(kind == 1) /*if is string command*/
				{
					strcpy(new->name,label);
					strcpy(new->str_value,str);
					new->int_values_num = values_num;
					new->address = DC;
					new->kind = kind;
						
				}
				else /*kind == 2*/
				{
					strcpy(new->name,label);
					memcpy(new->int_values,values,(sizeof(int)*values_num));
					new->int_values_num = values_num;
					new->address = DC;
					new->kind = kind;
				}
		cur->next = new; /*adding node to symbol list*/
		return 1;
	
	
	return 1;
}

void insert_command(command_struct * head,char * label,CommandInfo* commandInfo, int arguments_num, int** address,int line_number,struct argument_struct * arguments)
{
	command_struct * cur = head;
	int i;

	if(cur->next == NULL )
		{ 		
					/*insert data to new node */
					strcpy(cur->label,label);
					cur->commandInfo=commandInfo;
					cur->arguments_num=arguments_num;
					cur->address=(**address);
					cur->line_number=line_number;
		
					for(i=0; i < cur->arguments_num; i++)
						{
							strcpy(cur->arguments[i].argument_str , arguments[i].argument_str); 
							cur->arguments[i].addressingMode = arguments[i].addressingMode;
						}

					
		  
			/*creat new node for command list */
			cur->next = create_command_struct();
			if (cur->next == NULL)
				{
					printf("Failed to allocate command");
					return ;
				}
			
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
			for(i=0; i < cur->arguments_num; i++)
				{
					strcpy(cur->arguments[i].argument_str , arguments[i].argument_str); 
					cur->arguments[i].addressingMode = arguments[i].addressingMode;
				}
				  

		/* add the new node to the end of the list*/
		cur->next = create_command_struct();
		if (cur->next == NULL)
			{
				printf("Failed to allocate command");
				return ;
			}
 	}
}

void print_symbol_list(symbol_struct * cs)
{

	if(cs->name[0]== '\0')
	printf("\nSymbol list is empty\n");

	else
	while(cs)
	{
		printf("\n%s\n%d\n%d\n%d\n%d\n",cs->name,cs->value,cs->base_address,cs->offset,cs->kind);
		cs = cs->next;
	}


}

/*debug printing*/
void print_command_list(command_struct * head)
{
	int i;
	command_struct * cur = head;

	if( head->address == 0 && head->line_number == 0)
	printf("\nCommand list is empty\n");

	else
		{
			while(cur->next)
			{
				printf("\ncommand->label: %s\ncommand->name: %s\ncommand->arguments_num: %d\ncommand->address: %d\ncommand->line_number: %d\n",cur->label,cur->commandInfo->commandName,cur->arguments_num,cur->address,cur->line_number);
				for(i=0; i < cur->arguments_num; i++)
					 	printf("\n argument_str->> %s\n addressingMode->> %d\n", cur->arguments[i].argument_str ,cur->arguments[i].addressingMode);
					
				cur = cur->next;
			}
		}


}

void print_data_list(data_struct * head)
{
	int i;
	data_struct * cur = head;

	if((cur->str_value[0]=='\0') && (cur->int_values[0]==0))
	printf("\nData list is empty\n");

	else
		{
			while(cur)
			{
				if(cur->kind == 1)
				{
					printf("\ndata->name: %s\ndata->str_value: %s\ndata->int_values: -\ndata->int_values_num: %d\ndata->address: %d\n",cur->name,cur->str_value,cur->int_values_num,cur->address);
				printf("data->kind: %s\n","STRING");
				}
				
				else
				{
				printf("\ndata->name: %s\ndata->str_value: -\ndata->int_values: ",cur->name);
				for(i=0;i<cur->int_values_num;i++)
				{
				printf("%d ",cur->int_values[i]);
				}
				printf("\ndata->int_values_num: %d\ndata->address: %d\n",cur->int_values_num,cur->address);
				printf("data->kind: %s\n","DATA");
				}
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
					head->name[0]= '\0';						
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
					head->arguments_num=0;
					head->address = 0;
					head->line_number = 0;					
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
					head->int_values[0]=0;
					head->str_value[0]='\0';					
					temp = head;
					head = head->next;
					temp->next=NULL;
					free(temp);
				}
		}
}

void update_symbol_list(symbol_struct * symbol, int address)
{
	symbol_struct * head = symbol;
	
	while(head)
	{
		if(head->kind == DATA_SYMBOLKIND)
		{
			head->value += address;
			head->offset = ((head->value)%16);
			head->base_address = ((head->value) - (head->offset));
		}
		head = head->next;
	}
}

void update_data_list(data_struct * data, int address)
{
	data_struct * head = data;
	
	while(head)
	{
		head->address += address;
		head = head->next;
	}
	
}

int get_data_size(data_struct * data)
{
data_struct * head = data;
int size = 0;

while(head)
{
size += head->int_values_num;
head = head->next;
}
return size;
}



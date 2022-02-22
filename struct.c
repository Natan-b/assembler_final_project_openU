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

command_struct * create_command_struct()
{
	command_struct* command = malloc(sizeof(command_struct));
	command->label[0]='\0';
	command->arguments_num=0;
	command->address = 0;
	command->line_number = 0;
	command->next=NULL;
	return command;
}


data_struct * create_data_struct()
{
	data_struct* data = malloc(sizeof(data_struct));
	data->name[0]='\0'; 
	data->str_value[0]='\0';
	data->int_values[0]=0;
	data->int_values_num=0;
	data->address=0;
	data->next=NULL;
	return data;

}


/*init command struct*/
/*void init_command_struct(command_struct * command)
{
	command->label[0]='\0';
	command->arguments_num=0;
	command->address = 0;
	command->line_number = 0;
	command->next=NULL;
	

}*/

/*init data struct*/
/*void init_data_struct(data_struct *data)
{
	data->name[0]='\0'; 
	data->str_value[0]='\0';
	data->int_values[0]=0;
	data->int_values_num=0;
	data->address=0;
	data->next=NULL;
}*/

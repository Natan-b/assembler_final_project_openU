#ifndef STRUCT_H
#define STRUCT_H

#include "constants.h"



/*macro struct*/
typedef struct Node_name * ptr_macro_name;
typedef struct Node_name{
	char macro_line[MAX_ONE_LINE];
	ptr_macro_name next;
	}macro_name_node;

/*symbol struct*/
typedef struct Symbol_struct * symbol_ptr;
typedef struct Symbol_struct {
	char name[SYMBOL_MAX_LEN];
	/*address of symbol*/
	int value;
	/*base_address of symbol*/
	int base_address;
	/*offset*/
	int offset;
	/*bits array for kind of symbol*/
	SymbolKind kind;
	/*pointer to next node*/
	symbol_ptr next;
} symbol_struct;

/*struct for the information of arguments in command*/
typedef struct
{
	/*number of valid adrresing-mode for this argument*/
	int legalAddressingModesNum;
	/* the specific adreesing-mode for this argument*/
	int legalAddressingModes[ADDRESSING_MODES_NUM];
	/*argument is source or target*/
	ArgumentKind argumentKind;
} ArgumentInfo;

/* struct for information for each command type */
typedef struct
{
	/*type of the command*/
	CommandType commandType;
	/*name of command*/
	char* commandName;
	/*opp-code*/
	int oppCode;
	/*funct*/
	int funct;
	/*number of leagal args*/
	int argumentInfosNum;
	/*information of args*/
	ArgumentInfo argumentInfos[MAX_ARGUMENTS_NUMBER];
} CommandInfo;

/*Data struct*/
typedef struct Data_struct * data_ptr;
typedef struct Data_struct {
	/*name (if no have symbol->\0)*/
	char name[MAX_ONE_LINE]; 
	/*the string (if ist string)*/
	char str_value[MAX_ONE_LINE];
	/*the int value (if data)*/
	int int_values[MAX_INTS_IN_DATA];
	/*number of values for data*/
	int int_values_num;
	/*addres of data*/
	int address;
	/*kind of data*/
	DataKind kind;
	/*pointer to next node*/
	data_ptr next;
} data_struct;

/*struct for hold args that read from the file*/
typedef struct argument_struct {
	char argument_str[MAX_ARGUMENT_LEN];
	AddressingMode addressingMode;
} argument_struct;

/*Comand struct*/
typedef struct command_struct * command_ptr;
typedef struct command_struct {
	char label[SYMBOL_MAX_LEN];
	/*valid info for this command*/
	CommandInfo* commandInfo;
	/*num of args*/
	int arguments_num;
	/*address of command*/
	int address;
	/*number of line the command take in file*/
	int line_number;
	/* the arguments */
	struct argument_struct arguments[MAX_ARGUMENTS_NUMBER];
	/*pointer to next node*/
	command_ptr next;
} command_struct;


/*creating and initializing symbol struct*/
symbol_struct * create_symbol_struct();

/*creating and initializing command struct*/
command_struct * create_command_struct();

/*creating and initializing data struct*/
data_struct * create_data_struct();



#endif

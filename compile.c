
#include "preprocessor.h"
#include "general_functions.h"
#include "hash.h"
#include "struct.h"
#include "list.h"
#include "constants.h"
#include "compile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/* a "table" of the info of each optional command */
CommandInfo commandInfos[] =
{
	{MOV, "mov", 0, 0, 2,
		{{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, SOURCE},
		{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{CMP, "cmp", 1, 0, 2,
		{{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, SOURCE},
		{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, TARGET}}},
	{ADD, "add", 2, 10, 2,
		{{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, SOURCE},
		{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{SUB, "sub", 2, 11, 2,
		{{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, SOURCE},
		{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{LEA, "lea", 4, 0, 2,
		{{2, {DIRECT, INDEX}, SOURCE},
		{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{CLR, "clr", 5, 10, 1,
		{{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{NOT, "not", 5, 11, 1,
		{{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{INC, "inc", 5, 12, 1,
		{{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{DEC, "dec", 5, 13, 1,
		{{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{JMP, "jmp", 9, 10, 1,
		{{2, {DIRECT, INDEX}, TARGET}}},
	{BNE, "bne", 9, 11, 1,
		{{2, {DIRECT, INDEX}, TARGET}}},
	{JSR, "jsr", 9, 12, 1,
		{{2, {DIRECT, INDEX}, TARGET}}},
	{RED, "red", 12, 0, 1,
		{{3, {DIRECT, INDEX, REGISTER}, TARGET}}},
	{PRN, "prn", 13, 0, 1,
		{{4, {IMMEDIETE, DIRECT, INDEX, REGISTER}, TARGET}}},
	{RTS, "rts", 14, 0, 0},
	{STOP, "stop", 15, 0, 0}
};

void compile_file(char * file_name)
{
char full_line[MAX_ONE_LINE];
char line[MAX_ONE_LINE];
char word[MAX_WORD];
char preprocess_file_name[MAX_NAME_FILE];
char ob_file_name[MAX_NAME_FILE];
char ext_file_name[MAX_NAME_FILE];
char ent_file_name[MAX_NAME_FILE];
int i;
int label_flag;
int line_number = 0; /*count line from file to print if line have error*/
int IC = 100; /*command counter*/
int DC = 0; /*data counter*/
int ok = 1;
FILE * fd,* ob_file, *ext_file, *ent_file;


/*creating symbol,command and data lists*/
symbol_struct *symbol = create_symbol_struct();
command_struct *command = create_command_struct();
data_struct *data = create_data_struct();

sprintf(preprocess_file_name,"%s.am",file_name);
fd = fopen(preprocess_file_name,"r");

/*opening processed file*/

	if(fd == NULL)
	{
		printf("\ncannot open file %s\n", file_name);
		return;
	}
	
	while(1)
		{
			i=0;
			label_flag = 0;		
			/* get new line from the file */
			fgets(full_line,MAX_ONE_LINE,fd);
			if(feof(fd))
				break;
			line_number++;
			if(strlen(full_line) > 80)
			{
				printf("\nERROR (line %d): ilegal length of line\n", line_number);
				ok = 0;
				continue;
			}
			
			/*remove extra whitespaces and tabs from beginnig of string */
			clean_line(full_line,line);

			/*checking if line is an empty line*/
			if(strlen(line) == 0 )
				continue; /*will skip to the next line*/
			if(is_empty_line(line,i))
				continue; /*will skip to the next line*/

			get_word(line,i,word); /*recieving word*/

			/*checking if line is a comment line*/
			if(is_comment(line))
			{
				continue; /*will skip to the next line*/
			}
			
		if(word[strlen(word)-1] == ':') /*potentail label definition*/
		{
			/*checking if first word is a label definition or external label definition*/
			if(is_label_def(word,line_number, &ok))
			{
				if(analyze_label_type(symbol,line,word, line_number,DC,IC))
					label_flag = 1;
				else
				{
					ok = 0;
					continue; /*skip to next line in file*/
				}
				
				/*skipping to next char after ':'*/
				while(line[i] != ':')
				{
					i++;
				}
				i++; 
				
				if(is_empty_line(line,i)) /*if there is nothing after the symbol definition*/
				{
					printf("\nERROR (line %d): label without data or code is not allowed\n", line_number);
					ok = 0;
					continue;
				}
				
				get_word(line,i,word); /*getting next word in line*/	
			}
			else
			continue;
		}
			
				
			if(word[0] == '.')
			{
				/*analyzing data line*/
				ok = ok & analyze_data(data,line,word,line_number,label_flag,symbol,&DC);
			}
			else
			/*analyzing command line*/
			ok = ok & analyze_cmd(command,line,word,line_number,label_flag,&IC);
						
		}
		
	/*updating addresses on symbol and data list after running through file the first time*/
	update_symbol_list(symbol,IC);
	update_data_list(data,IC);

	
rewind(fd);
	/* now performing second check */
	line_number = 0;
	while(1)
		{
			
			i=0;
			label_flag = 0;		
			/* get new line from the file */
			fgets(full_line,MAX_ONE_LINE,fd);
			if(feof(fd))
				break;
			line_number++;
			
			/*remove extra whitespaces and tabs from beginnig of string */
			clean_line(full_line,line);

			/*checking if line is an empty line*/
			if( strlen(line) == 0 )
				continue; /*will skip to the next line*/
		
			get_word(line,i,word); /*recieving word*/
			
			if(is_comment(line))
			{
				continue; /*will skip to the next line*/
			}
			
			/*checking if first word is a label definition or external label definition*/
			if(word[strlen(word)-1] == ':')
			{
				while(line[i] != ':')
				{
					i++;
				}	
				i++; /*to get to char after ':'*/
			}
			
			get_word(line,i,word); /*recieving word*/
			
			if(strcmp(word,".entry") == 0)
			{
				while(line[i] != 'y')
				{
					i++;
				}
				i++;
				
				get_word(line,i,word);
				
				if(is_label(word))
				{
					if(label_check(word)) /*checking if isn't regsiter\command name*/
					{
						/*checking if entry label exists in symbol list*/
						if(!update_symbol_entry(symbol,word,line_number))
							ok = 0;
					}
					else
					{
						printf("\nERROR (line %d): symbol name is a defined command/register name\n", line_number);
						ok = 0;
					}
				}
				else
				{
					printf("\nERROR (line %d): word after entry command is not a symbol\n", line_number);
					ok = 0;
				}	
			}
			continue;		
		}
	

	fclose(fd);
		
	/*checking that all symbols in command lines are existing symbols in symbol list*/
	if(!check_command_symbols(command,symbol,line_number))
		ok = 0;

	/*check if memory is too big */
	if( !(check_memory(data, command)) )
		{
			ok = 0;
			printf("\nERROR : memory space is full\n");
		}
	if(ok)
		{
			/* write the ob file */
			sprintf(ob_file_name, "%s.ob", file_name);
			ob_file = fopen(ob_file_name, "w");
			if (ob_file != NULL)
				{
					write_ob_file(ob_file, command, data, symbol);
					fclose(ob_file);
				}
			else
				{
					printf("OB file %s cannot be created\n", ob_file_name);
				}

			/* write the ext file */
			sprintf(ext_file_name, "%s.ext", file_name);
			ext_file = fopen(ext_file_name, "w");
			if (ext_file != NULL)
				{
					write_ext_file(ext_file, command, symbol);
					fclose(ext_file);
				}
			else
				{
					printf("EXT file %s cannot be created\n", ext_file_name);
				}

			/* write the ent file */
			sprintf(ent_file_name, "%s.ent", file_name);
			ent_file = fopen(ent_file_name, "w");
			if (ent_file != NULL)
				{
					write_ent_file(ent_file, symbol);
					fclose(ent_file);
				}
			else
				{
					printf("ENT file %s cannot be created\n", ent_file_name);
				}
		printf("Finish processing assembler file '%s' successfully\n", file_name);
		}
	else
		printf("Finish processing assembler file '%s' with errors\n", file_name);

	/*freeing allocated memory*/
	free_command_list(command);
	free_data_list(data);
	free_symbol_list(symbol);		
}

/*this function will copy first word from/after index i in 'from' array to 'to' array*/
void get_word(char * from,int i,char * to)
{
	int j = 0;

	while(from[i] == ' ' || from [i] == '\t')
	{
		i++;
	}
	while(from[i] != ' ' && from[i] != '\t' && from[i] != '\n')
	{
		to[j++] = from[i++];
	}
	
	to[j] = '\0';
}


/*function will check if line is a comment line*/
int is_comment(char * line)
{
	if(line[0] == ';')
	
		return 1;
	else 
		return 0;
		
}


/*function will check if line is an empty line*/
int is_empty_line(char * line, int i)
{

	while(spaceOrTab(line[i]))
	{
		i++;
	}
	
	if(line[i] == '\0')
		return 1;
	else
		return 0;
}

/*this function will check to see if the given word is a label definition*/
int is_label_def(char * word, int line_number, int * ok)
{
	int i = 0;
	/*first letter is capital or small letter*/
	if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
	{
		i++;
		/*the rest of the chars are letters or numbers*/
		while((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= '0' && word[i] <= '9'))
			{
				i++;
			}
		/*check if word ends with ':'*/
		if(word[i] == ':' && word[i+1] == '\0')
		{
			/*checking if symbol is legal length*/
			if(strlen(word) <= SYMBOL_MAX_LEN)
				return 1;
			else
			{
				printf("\nERROR (line %d): symbol definitiom is too long\n", line_number);
				*ok = 0;
				return 0;
			}
		}
		else
		{	
			printf("\nERROR (line %d): ilegal symbol name\n", line_number);
			*ok = 0;
			return 0;
		}	
	}
	printf("\nERROR (line %d): ilegal symbol name\n", line_number);
	*ok = 0;
	return 0;
}

/*this function will check to see if the given word is a label*/
int is_label(char * word)
{
	int i = 0;
	/*first letter is capital or small letter*/
	if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
	{
		i++;
			/*the rest of the chars are letters or numbers*/
		while((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= '0' && word[i] <= '9'))
			{
				i++;
			}
			
		if(word[i] == '\0')
		{
			if(strlen(word) <= SYMBOL_MAX_LEN) /*checking if symbol is legal length*/
			return 1;
		}	
		else
			return 0;
			
	}
	return 0;
}

/*function checks to see if given label is not a register\command name*/
int label_check(char * label)
{
int i;

		/*removing ':' sign from symbol (if there is one)*/
	if(label[strlen(label)-1] == ':')
		label[strlen(label)-1] = '\0';
	
	/*checking if label is a command name*/	
	for(i=0;i<16;i++)
	{
		if(strcmp(commandInfos[i].commandName,label) == 0)
		return 0;
	}
		
	/*checking if label is a register name*/
	if((strcmp(label,"r0") == 0) || (strcmp(label,"r1") == 0) || (strcmp(label,"r2") == 0) || (strcmp(label,"r3") == 0) || (strcmp(label,"r4") == 0) || (strcmp(label,"r5") == 0) || (strcmp(label,"r6") == 0) || (strcmp(label,"r7") == 0) || (strcmp(label,"r8") == 0) || (strcmp(label,"r9") == 0) || (strcmp(label,"r10") == 0) || (strcmp(label,"r11") == 0) || (strcmp(label,"r12") == 0) || (strcmp(label,"r13") == 0) || (strcmp(label,"r14") == 0) || (strcmp(label,"r15") == 0))
		return 0;
	
	return 1;
}


/*function analyzes the type of label before inserting into symbol list*/
int analyze_label_type(symbol_struct *symbol, char * line, char * label, int line_number, int DC, int IC)
{
	int i = 0;
	char word_2[MAX_WORD];
	
		/*checking if label isn't a command\register name*/
		if (label_check(label) == 0) 
		{
			printf("\nERROR (line %d): symbol name is a defined command/register name\n",line_number);
				return 0;
		}

	/*skipping to next char after ':'*/
		while(line[i] != ':')
		{
			i++;
		}
		i++;
	
		get_word(line,i,word_2); /*getting next word in line to determine type of label*/

		if(strcmp(word_2,".data") == 0)
		{
			/*inserting into symbol list as data symbol kind*/
			if(insert_symbol(symbol,label,DC,DATA_SYMBOLKIND))
			return 1;
			else
			{
				printf("\nERROR (line %d): symbol already exists in list\n",line_number);
				return 0;
			}
		}
			
		if(strcmp(word_2,".string") == 0)
		{
			/*inserting into symbol list as data symbol kind*/
			if(insert_symbol(symbol,label,DC,DATA_SYMBOLKIND))
			return 1;
			else
			{
				printf("\nERROR (line %d): symbol already exists in list\n",line_number);
				return 0;
			}
		}
			
		if(strcmp(word_2,".extern") == 0)
		{
			printf("\nWARNING (line %d): label definition cannot appear before external command\n", line_number);
			return 1;
		}
		
		if(strcmp(word_2,".entry") == 0)
		{
			printf("\nWARNING (line %d): label definition cannot appear before entry command \n", line_number);
			return 1;
		}
			
		/*in any other case it is a code label line*/
		/*inserting into symbol list as code symbol kind*/
		if(insert_symbol(symbol,label,IC,CODE_SYMBOLKIND))
			return 1;
		else
		{
			printf("\nERROR (line %d): symbol already exists in list\n",line_number);
			return 0;
		}
}

/*function analyzes possible data line and passes on for further inspection*/
int analyze_data(data_struct * data ,char * line, char * word, int line_number,int label_flag,symbol_struct * symbol, int * DC)
{
	int i =0;
	
	
	if(label_flag)
	{
		while(line[i] != ':')
		{
			i++;
		}	
			i++; /*to get to char after ':'*/
	}
	
	/*if is entry label line will skip*/
	if(strcmp(word,".entry") == 0)
		return 1;
	
	/*if is external label line*/
	if(strcmp(word,".extern") == 0)
	{
		/*skipping ".extern" command*/
		while(line[i] != 'n') 
		{
			i++;
		}	
			i++; /*to get to char after 'n'*/
			
		get_word(line,i,word); /*recieving potential symbol*/
		
		if(is_label(word))
		{
			if(label_check(word))
			{
				/*inserting into symbol list as external symbol kind*/
				if(insert_symbol(symbol,word,0,EXERNAL_SYMBOLKIND))
					return 1;
				
				else
				{
				   printf("\nERROR (line %d): symbol already exists as local symbol\n",line_number);
				   return 0;
				}
			}
			else
			{
				printf("\nERROR (line %d): symbol name is a defined command/register name\n",line_number);
				return 0;
			}
		}
		printf("\nERROR (line %d): not a legal symbol name\n", line_number);
		return 0;
	}
	
	if(strcmp(word,".string") == 0)
	{
		/*sending to function for further inspection*/
		if(analyze_string_cmd(data,line,label_flag,line_number,DC))
			return 1;
		else
		{
			return 0;
		}
	}
		
		if(strcmp(word,".data") == 0)
		{
			/*sending to function for further inspection*/
			if(analyze_data_cmd(data,line,label_flag,line_number,DC))
				return 1;
			else
				return 0;
		}
		/*if none of the above options then is unknown command error*/
		printf("\nERROR (line %d): '%s' is unknown command\n", line_number, word);
		return 0;
		
	}

/*function analyzes possible coomand line and passes on for further inspection*/
int analyze_cmd(command_struct * command, char * line, char * word, int line_number,int label_flag, int * IC)
{
int i =0,j,k, found;
CommandInfo* commandInfo;
command_struct* temp = command;

commandInfo = is_cmd(word); /*checking if word is listed in command info struct*/
if(commandInfo == NULL )
	{
		printf("\nERROR (line %d): '%s' is unknown command\n", line_number, word);
		return 0;
	}

if(label_flag) /*if label appears at beginning of line then skip label*/
	{
		while(line[i] != ':')
		{
			i++;
		}
			
			i++; /*to get ot char agter ':'*/
	}

while(spaceOrTab(line[i])) i++;

i = i + strlen(word); /*pointing index to arguments after command name*/

while(temp->next!=NULL)
	temp=temp->next;
		
if(!(fill_arguments(line_number, (line + i), temp))) /*send to function to fill in the argumaents information*/
	return 0;

/* check for bad numbers of adressing mode*/
if(commandInfo-> argumentInfosNum != temp->arguments_num)
	{
		printf("\nERROR (line %d): Too many/less arguments in command '%s' \n", line_number, word);
		return 0;	
	}

/* check for bad adressing mode referd for target and surse*/
for(j=0; j < temp->arguments_num ; j++)
	{
		found=0;
		for (k = 0; k < commandInfo->argumentInfos[j].legalAddressingModesNum; k++)
			{
				if (temp->arguments[j].addressingMode ==
					commandInfo->argumentInfos[j].legalAddressingModes[k])
					{
						found = 1;
						break;
					}
			}
	if(!found)
		{
			printf("\nERROR (line %d): Bad addressing mode for argument \n", line_number);
			return 0;
		}
	}


insert_command(command,line,commandInfo,temp->arguments_num,&IC,line_number,temp->arguments);
(*IC) += get_command_size(temp);
return 1;
}

/*function calculates the size in "words" that the given command takes*/
int get_command_size(command_struct* cur)
{
	int size = 0;
	int i;
	if (cur->arguments_num == 0) /*if there are no argument in command*/
	{
		return 1;
	}
	if (cur->arguments_num == 1) /*if there is only one argument in command*/
	{	
		size+=2;
		switch(cur->arguments[0].addressingMode) /*ckecking argumetnts addressing mode*/
			{
				case IMMEDIETE:
					size += 1;
					break;
	
				case DIRECT:
					size += 2;		
					break;

				case INDEX:
					size += 2;
					break;
	
				case REGISTER:
					break;
				

			}
		return size;
	}
	if (cur->arguments_num == 2) /*if there are 2 argument in command*/
	{
		size+=2;
		for(i=0; i < cur->arguments_num; i++)
			{
				switch(cur->arguments[i].addressingMode) /*ckecking argumetnts addressing mode*/
					{
						case IMMEDIETE:
							size += 1;
							break;
	
						case DIRECT:
							size += 2;
							break;

						case INDEX:
							size += 2;
							break;
	
						case REGISTER:
							break;
					}

			}
		return size;
	}
	return 1;
}

/*function checks if given word is a command name and return the command's information*/
CommandInfo* is_cmd(char *word)
{
int i;
/*running through command info struct and checking if given word matches*/
for(i=0; i<=15; i++)
	{
		if(strcmp(word,commandInfos[i].commandName)==0) 
			return &commandInfos[i];
	}
return NULL;
}

/*function arranges the arguments from the line and checks for errors*/	
int fill_arguments(int line_number, char* line, command_struct* command)
{
	int i = 0, j = 0, k;


	while (line[i] && j < MAX_ARGUMENTS_NUMBER)
	{
		while ((line[i] != '\0') && spaceOrTab(line[i]))
			i++;
		if (line[i] == '\0')
			break;
		if (line[i] == ',')
		{
			if (j == 0)
			{
				printf("\nERROR (line %d): Extra comma before arguments\n", line_number);
				return 0;
			}
			else
			{
				i++;
			}
			while ((line[i] != '\0') && spaceOrTab(line[i]))
				i++;
		}
		k = 0;
		/* get argument */
		while ((line[i] != '\0') && !spaceOrTab(line[i]) && (line[i] != ','))
		{
			command->arguments[j].argument_str[k] = line[i];
			k++;
			i++;
		}
		command->arguments[j].argument_str[k] = '\0';

		if(strlen(command->arguments[j].argument_str) > SYMBOL_MAX_LEN)
			{
				printf("\nERROR (line %d): Ilegal argument\n", line_number);
				return 0;
			}

		if (strlen(command->arguments[j].argument_str) == 0)
		{
			printf("\nERROR (line %d): Empty argument\n", line_number);
			return 0;
		}
		
		/* get the addressing mode for the argument */
		if (!fill_addressing_mode(&command->arguments[j]))
		{
			printf("\nERROR (line %d): Bad addressing mode for argument\n", line_number);
			return 0;
		}
		j++;
	}

	while ( spaceOrTab(line[i]))
			i++;
	if (line[i]!='\0')
		{
			printf("\nERROR (line %d): bad comma in arguments\n", line_number);
			return 0;
		}	
	
	if (j > 0)
	{
		
		command->arguments_num = j;
	}
	return 1;

}	

/*function fills the adrresing mode each command line arguments*/
int fill_addressing_mode(argument_struct* argument)
{
	
	if (fill_register_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_direct_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_index_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_immediete_addressing_mode(argument))
	{
		return 1;
	}
	return 0;
}

/*function checks if arguments in line are of immediate addressing mode*/
int fill_immediete_addressing_mode(argument_struct* argument)
{
	int succeded,num,i=0;
	if (strlen(argument->argument_str) < 2)
		return 0;
	if (argument->argument_str[0] != '#')
		return 0;
	/*delete the # from the begin of the line*/
	move_left(argument->argument_str, 1);
	/* not use the returned value because this is just a check
	if there is a number in argument */
	if(argument->argument_str[0] == '-' || argument->argument_str[0] == '+')
		i++;

	for(; i < strlen(argument->argument_str); i++)	
		{ 
			if(!is_number(argument->argument_str[i]))
				{
					return 0;
				}
		}

	num = get_number_from_string(argument->argument_str, &succeded);
	/* max number for immediate is 16 bit wide = 65536*/
	if( num > THRESHOLD_IMMEDIATE_NUM)
		return 0;

	if (succeded)
		argument->addressingMode = IMMEDIETE;
	return succeded;
}

/*function checks if arguments in line are of register addressing mode*/
int fill_register_addressing_mode(argument_struct* argument)
{
	int num, succeded;
	/* registers can be from 'r0' to 'r15' only */
	if (strlen(argument->argument_str) == 1) 
		return 0;	
	if (strlen(argument->argument_str) > 3)
		return 0;
	if (argument->argument_str[0] != 'r')
		return 0;

	num = get_number_from_string(&argument->argument_str[1], &succeded);
	if (!succeded)
		return 0;
	/* check number for register is ok*/
	if ((num < 0) || (num > 15))
		return 0;
	/*clean and leave just the number of the argument*/
	move_left(argument->argument_str, 1);
	argument->addressingMode = REGISTER;
	return 1;
}


/*function checks if arguments in line are of direct addressing mode*/
int fill_direct_addressing_mode(argument_struct* argument)
{
	/* check if argument is legal lablbe */
	if (!is_label(argument->argument_str))
		return 0;

	argument->addressingMode = DIRECT;
	return 1;
}

/*function checks if arguments in line are of direct addressing mode*/
int fill_index_addressing_mode(argument_struct* argument)
{
	/* check for minimum characrets for index. example: 'X[r10]' */
	if (strlen(argument->argument_str) < MAX_REGISTER_LEN)
		return 0;
		
	if( !(symbol_and_register_is_ligal(argument->argument_str)))
		return 0;
	
	argument->addressingMode = INDEX;
	return 1;
}


/*function checks if symbol and register are legal in direct addressing mode format*/
int symbol_and_register_is_ligal(char* word)
{
	int i=0;
	int j=0;
	int num;
	char symbol_str[SYMBOL_MAX_LEN];
	char register_str[MAX_REGISTER_LEN];
	
	/* get the symbol */
	while(word[i] != '\0' && word[i] != '[')
		symbol_str[j++]=word[i++];
	
	symbol_str[j]='\0';

	if(!(is_label(symbol_str)))
		return 0;
	if(!(label_check(symbol_str)))
		return 0;
	j=0;
	/* get the register */
	if(word[i] != '[')
		return 0;
	i++;

	if(word[i] != 'r')
		return 0; 
	i++;

	while(is_number(word[i]) == 1 && word[i] != ']' && word[i] != '\0')
		register_str[j++] = word[i++];
	register_str[j]='\0';

	if(word[i]!=']')
		return 0;	
	num = atoi(register_str);
	/* check the number of the register */
	if ((num < 10) || (num > 15))
		return 0;

	return 1;
}

/*function gets the symbol and register from line (in direct addressing mode format line)*/
void get_symbol_and_register(char* word, char* register_str, char* symbol_str)
{
	int i=0;
	int j=0;
	while(word[i] != '\0' && word[i] != '[')
		symbol_str[j++]=word[i++];	
	symbol_str[j]='\0';

	j=0;
	i+=2;

	while( word[i] != ']' && word[i] != '\0')
		register_str[j++] = word[i++];
	register_str[j]='\0';
}

/*function analyzes string data command before inserting into data list*/
int analyze_string_cmd(data_struct * data, char * line,int label_flag, int line_number, int * DC)
{
	int count = 0;
	int i = 0, j = 0;
	char str[MAX_WORD];
	char label[SYMBOL_MAX_LEN];
	
	if(label_flag)
	{
		get_word(line,i,label); /*storing symbol in 'label'*/
		label[strlen(label)-1] = '\0'; /*getting rid of ':'*/
		
		while(line[i] != ':') /*skipping label*/
		{
			i++;
		}
		i++; /*char after label*/
	}
		
	/*skipping to index after word ".string"*/
	while(line[i] != 'g')
	{
		i++;
	}
	i++;
		
	while (spaceOrTab(line[i]))
	{
		i++;
	}
		
	if(line[i] == '\0')
	{
		printf("\nERROR (line %d): missing string in line\n", line_number);
		return 0;
	}
		
	if (line[i] != '"')
	{
		printf("\nERROR (line %d): missing quoation mark in string\n", line_number);
		return 0;
	}
	
	i++;
	
	while ((line[i] != '\0') && (line[i] != '"')) /*pulling string data from line*/
	{
		str[j++] = line[i++];
		count++;
	}
	str[j] = '\0';
	count++;
		
	if (line[i] != '"')
	{
		printf("\nERROR (line %d): missing quoation mark in string\n", line_number);
		return 0;
	}
	
	i++;
	while ((line[i] != '\0') && spaceOrTab(line[i]))
		i++;
	if (line[i] != '\0')
	{
		printf("\nERROR (line %d): extra character after string\n", line_number);
		return 0;
	}
	/*inserting recieved string data into data list*/
	if(insert_data(data,line,str,NULL,count,*DC,STRING_DATAKIND))
	{
		*DC += strlen(str)+1; /*updating DC counter*/
		return 1;
	}
	else
	{
		printf("\nERROR (line %d): failed to allocate data\n", line_number);
		return 0;
	}
}

/*function analyzes data data command before inserting into data list*/
int analyze_data_cmd(data_struct * data, char * line, int label_flag,int line_number, int * DC)
{
	int count = 0;
	int i = 0;
	int values[MAX_WORD];
	char label[SYMBOL_MAX_LEN];
	label[0] = '\0';
	                             
	if(label_flag)
	{
		get_word(line,i,label); /*storing symbol in 'label'*/
		label[strlen(label)-1] = '\0'; /*getting rid of ':'*/
		
		while(line[i] != ':') /*skipping label*/
		{
			i++;
		}
		i++; /*char after label*/
	}
	
	/*skipping to index after word ".data"*/
	while(line[i] != '.')
	{
		i++;
	}
	
	i += 5;
	
	
	while (spaceOrTab(line[i]))
	{
		i++;
	}
	
	if(line[i] == '\0')
	{
		printf("\nERROR (line %d): missing data in line\n", line_number);
		return 0;
	}
	/*checks and retrives numbers from data line*/
	if(fill_numbers(line,i,values,line_number,&count))
	{
		/*inserting recieved data into data list*/
		if(insert_data(data,line,NULL,values,count,*DC,DATA_DATAKIND))
		{
			*DC += count; /*updating data counter*/
			return 1;
		}
		else
		{
			printf("\nERROR (line %d): failed to allocate data\n", line_number);
			return 0;
		}
	}
	return 0;
}
	
/*this function will recieve the .data line and extract the numbers into an int array*/
int fill_numbers(char * line, int i, int * values, int line_number, int * count)
{
	char num_str[MAX_WORD];
	int j, h = 0;
	int succeeded, num;
		
	/*runs until end of the line*/
	while(line[i])
	{
		j = 0;
		
		if(line[i] == ',') /*comma before a number*/
		{
			printf("\nERROR (line %d): ilegal use of comma\n", line_number);
			return 0;
		}
			
		while (line[i] && spaceOrTab(line[i]))
			i++;
			
		while (line[i] && (line[i] != ',') && !spaceOrTab(line[i])) /* extracting potential number*/
			num_str[j++] = line[i++];
		
		while (line[i] && spaceOrTab(line[i]))
			i++;
				
		if (line[i] && (line[i] == ','))
		{
			i++;
			
			while (line[i] && spaceOrTab(line[i]))
				i++;
				
			if(!line[i]) /*if line ends after comma*/
			{
				printf("\nERROR (line %d): ilegal comma at end of data\n", line_number); /*needs to be number after comma*/
				return 0;
			}
		}
		else if(!line[i])
		{
			; /*do nothing*/
		}
		else 
		{
			printf("\nERROR (line %d): missing comma\n", line_number);
			return 0;
		}
		
		
			
				
		if(line[i] == ',') /*double comma*/
		{
			printf("\nERROR (line %d): ilegal use of comma\n", line_number);
			return 0;
		}
	
		num_str[j] = '\0';
			
		/*convert string to num*/
		num = get_number_from_string(num_str, &succeeded);
			
		if(succeeded) /*if num is indeed a number*/
		{
			values[h++] = num;
			(*count)++;
			continue;
		}
		else
		{
			printf("\nERROR (line %d): one of the data parametes is not a number\n", line_number);
			return 0;
		}	
	}
		return 1;
}

/*function creates the ob file and calls the print to file methode*/
void write_ob_file(FILE* ob_file, command_struct * command, data_struct * data, symbol_struct * symbol)
{
	command_struct * cur_command = command;
	data_struct * cur_data = data;

	int command_size=0, data_size = 0;
	/* get command size */
	while(cur_command->next != NULL)
		{
			command_size += get_command_size(cur_command);
			cur_command = cur_command->next;
		}
	/* get data size */
	data_size = get_data_size(data);

	fprintf(ob_file, "%d %d\n", command_size, data_size);

	cur_command = command;

	/* if command struct not emty, write command to .ob file */
	if( cur_command->address != 0 )
		{
			while( cur_command->next != NULL && cur_command->address != 0)
				{
					write_command_to_ob_file(ob_file, cur_command, symbol);
					cur_command = cur_command->next;
				}
	
		}

	/* if data struct not emty, write command to .ob file */
	if( cur_data->str_value[0] != '\0' || cur_data->int_values[0] != 0 )
		{
			while( cur_data ) 
				{
					write_data_to_ob_file(ob_file, cur_data);
					cur_data = cur_data->next;
				}
		}

}


/*function arranges the command and argument as wish and calls the print method*/
void write_command_to_ob_file(FILE* ob_file, command_struct* command, symbol_struct* symbol)
{
	int num, succeded, address = command->address, i;
	unsigned int word = 0;
	char index_symbol_str[SYMBOL_MAX_LEN];
	char index_register_str[MAX_REGISTER_LEN];
	symbol_struct* cur_symbol;
	/*arrange the bits of the command
	A - absolut = 0x40000, R - Relocatable = 0x20000, E - External = 0x10000*/
	word = word | 0x40000 ;
	word |= 1 << command->commandInfo->oppCode ;
	write_word(ob_file, address, word);

	if(command->commandInfo->argumentInfosNum != 0)
		{
			address++;
			word = word & 0x40000 ;
			if( command->commandInfo->funct != 0 )
				word |= command->commandInfo->funct << 12;
			if( command->arguments_num == 2 )
				{	
					/* SOURCE */
					if( command->arguments[0].addressingMode == REGISTER )
						{
							num = get_number_from_string(command->arguments[0].argument_str, &succeded);
							word |= num << 8;
						}
					if( command->arguments[0].addressingMode == INDEX )
						{
						 	get_symbol_and_register(command->arguments[0].argument_str, index_register_str, index_symbol_str);
							num = get_number_from_string(index_register_str, &succeded);
							word |= num << 8;
						}
					num = command->arguments[0].addressingMode;
					word |= num << 6;
					/* TARGET */
					if( command->arguments[1].addressingMode == REGISTER )
						{
							num = get_number_from_string(command->arguments[1].argument_str, &succeded);	
							word |= num << 2;
						}
					if( command->arguments[1].addressingMode == INDEX )
						{
						 	get_symbol_and_register(command->arguments[1].argument_str, index_register_str, index_symbol_str);
							num = get_number_from_string(index_register_str, &succeded);
							word |= num << 2;
						}
					num = command->arguments[1].addressingMode;
					word |= num;

					write_word(ob_file, address, word);
				}
			

			if( command->arguments_num == 1 )
				{
					/* TARGET */
					if( command->arguments[0].addressingMode == REGISTER )
						{
							num = get_number_from_string(command->arguments[0].argument_str, &succeded);	
							word |= num << 2;
						}
					if( command->arguments[0].addressingMode == INDEX )
						{
						 	get_symbol_and_register(command->arguments[0].argument_str, index_register_str, index_symbol_str);
							num = get_number_from_string(index_register_str, &succeded);
							word |= num << 2;
						}
					num = command->arguments[0].addressingMode;
					word |= num;

					write_word(ob_file, address, word);
				}
		
			/*arrange the arguments bits*/
			for (i = 0; i < command->arguments_num; i++)
					{
						word = 0;
						
						switch (command->arguments[i].addressingMode)
						{
							case REGISTER:
								break;
				
							case IMMEDIETE:
								word = word | 0x40000 ;
								num = get_number_from_string(command->arguments[i].argument_str, &succeded);
								word |= num;
								word = word & 0x4ffff ;
								address++;
								write_word(ob_file, address, word);
								break;

							case INDEX:
								cur_symbol = find_symbol(index_symbol_str, symbol);

								if (cur_symbol->kind == EXERNAL_SYMBOLKIND)
								{
									/* EXERNAL */
									word = word | 0x10000 ;
									address++;
									write_word(ob_file, address, word);
									address++;
									write_word(ob_file, address, word);
									
								}
								else
								{
									/* Relocatable */

									/* base_address */
									word = word | 0x20000 ;
									word |= cur_symbol->base_address;
									address++;						
									write_word(ob_file, address, word);
									
									/* offset */
									word = word & 0x20000 ;
									word |= cur_symbol->offset;
									address++;
									write_word(ob_file, address, word);
								}
								break;

							case DIRECT:
								cur_symbol = find_symbol(command->arguments[i].argument_str, symbol);
								if (cur_symbol->kind == EXERNAL_SYMBOLKIND)
								{
									/* EXERNAL */
									word = word | 0x10000 ;
									address++;
									write_word(ob_file, address, word);
									address++;
									write_word(ob_file, address, word);
									
								}
								else
								{
									/* Relocatable */

									/* base_address */
									word = word | 0x20000 ;
									word |= cur_symbol->base_address;
									address++;
									write_word(ob_file, address, word);
									
					
									/* offset */
									word = word & 0x20000 ;
									word |= cur_symbol->offset;
									address++;
									write_word(ob_file, address, word);
								}
								break;
						}
					
				} 

		}
	
}


/*function writes word to the ob file */
void write_word(FILE* file, int address, unsigned int word)
{
	unsigned int A, B ,C, D, E;
	/* masking */
	A = word & 0xf0000;
	B = word & 0xf000;
	C = word & 0xf00;
	D = word & 0xf0;
	E = word & 0xf;
	
	fprintf(file, "%04d A%x-B%x-C%x-D%x-E%x \n", address, A>>16 , B>>12, C>>8, D>>4, E);
}

/*function finds symbol name in the list of symbols and return address*/
symbol_struct* find_symbol(char* name,  symbol_struct * symbol)
{
	symbol_struct* cur = symbol;
	while(cur)
		{
			if (strcmp(cur->name,name) == 0)
				return cur;
			cur = cur->next;
		}
	return NULL;
}

/*this function will check if potentail symbold in command line exist in symbol list*/
int check_command_symbols (command_struct * command, symbol_struct * symbol, int line_number)
{
	command_struct * cur = command;
	int i;
	
	while(cur)
	{
		for(i=0; i<cur->arguments_num;i++) /*checking each argument*/
		{
			if(cur->arguments[i].addressingMode == DIRECT) /*if potential symbol*/
			{
				if(!find_symbol(cur->arguments[i].argument_str,symbol)) /*looking for symbol in symbol list*/
				{
					printf("\nERROR (line %d): symbol in command line does not exist in symbol list\n", line_number);
					return 0;
				}
			}
		}
		cur = cur->next; 
	}
	return 1;
}


/*function creates and writes the ent file*/
void write_ent_file(FILE* ent_file, symbol_struct * symbol)
{
	symbol_struct* cur = symbol;
	while( cur != NULL)
		{
			if (cur->kind == CODE_ENTRY_SYMBOLKIND || cur->kind == DATA_ENTRY_SYMBOLKIND)
				fprintf(ent_file, "%s,%d,%d\n", cur->name, cur->base_address , cur->offset);
			cur = cur->next;
		}
	
}

/*function creates and writes the ext file*/
void write_ext_file(FILE* ext_file, command_struct * command, symbol_struct * symbol)
{
	
	command_struct* cur_command = command;
	symbol_struct* cur_symbol = symbol;
	int i;

	while( cur_command != NULL)
		{
			for(i=0; i < cur_command->arguments_num;i++) /*checking each argument*/
				{
					if(cur_command->arguments[i].addressingMode == DIRECT) /*if potential symbol*/
						{
							cur_symbol = find_symbol(cur_command->arguments[i].argument_str, symbol);	
							if( cur_symbol->kind == EXERNAL_SYMBOLKIND )
								{
									if( i == 0)
										{
											fprintf(ext_file, "%s BASE %d\n", cur_symbol->name, cur_command->address + i + 2);
											fprintf(ext_file, "%s OFFSET %d\n\n", cur_symbol->name, cur_command->address + i + 3);
										}
									else
										{
											fprintf(ext_file, "%s BASE %d\n", cur_symbol->name, cur_command->address + i + 1);
											fprintf(ext_file, "%s OFFSET %d\n\n", cur_symbol->name, cur_command->address + i + 2);
										}
								}
						}
				}
			
			cur_command = cur_command->next;
		}
}

/*function translates data to ob format and writes it in file*/
void write_data_to_ob_file(FILE* ob_file, data_struct* cur_data)
{
	unsigned int word = 0;
	int i;

	word = word | 0x40000 ;

	switch (cur_data->kind)
	{
	case DATA_DATAKIND:
		for (i = 0; i < cur_data->int_values_num; i++)
		{
			word = word & 0x40000 ;
			word |= cur_data->int_values[i];
			word = word & 0x4ffff ;
			write_word(ob_file, cur_data->address + i, word);
		}
		break;	
	case STRING_DATAKIND:
		for (i = 0; i < cur_data->int_values_num -1; i++)
		{
			word = word & 0x40000 ;
			word |= cur_data->str_value[i];
			write_word(ob_file, cur_data->address + i, word);
		}
		word = word & 0x40000 ;
		write_word(ob_file, cur_data->address + i , word);
		break;
	default:
		break;
	}
}

/*function checks if given file exceeeds memeroy capacity in computer*/
int check_memory(data_struct* data, command_struct* command)
{
	int num=0;
	data_struct* cur_data = data;
	command_struct* cur_command = command;
	/* check if data struct is empty, if it is, check command struct memory size */
	if((cur_data->str_value[0]=='\0') && (cur_data->int_values[0]==0))
		{
			while(cur_command->next != NULL)
		{
			num += get_command_size(cur_command);
			cur_command = cur_command->next;
		}
			
			
			if( num > MAX_MEMORY_NUM )
				return 0;
		}
	else
		{
			while(cur_data->next) /*runs until the end of the list*/
				cur_data= cur_data->next;
				
			num = cur_data->int_values_num + cur_data->address; /*gets the int of last line of memery*/
			
			if( num > MAX_MEMORY_NUM )
				return 0;
			
		}
	return 1;
}

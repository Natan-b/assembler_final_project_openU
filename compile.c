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
int i;
int label_flag;
int line_number = 0; /*count line from file to print if line have error*/
int IC = 100;
int DC = 0;
int ok = 1;
FILE * fd;

/*creatinf symbol,command and data lists*/
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
	printf("start file: %s\n",file_name);
	while(1)
		{
			i=0;
			label_flag = 0;		
			/* get new line from the file */
			fgets(full_line,MAX_ONE_LINE,fd);
			if(feof(fd))
				break;
			line_number++;
			if(strlen(full_line) > 81)
			{
				printf("\nERROR (line %d): ilegal length of line\n", line_number);
				ok = 0;
				continue;
			}
			/*remove extra whitespaces and tabs from beginnig of string */
			clean_line(full_line,line);
			
			/*checking if line is an empty line*/
			if(is_empty_line(line))
			continue; /*will skip to the next line*/
			
			get_word(line,i,word); /*recieving word*/

			/*checking if line is a comment line*/
			if(is_comment(line))
			{
				continue; /*will skip to the next line*/
			}
			
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
		
				get_word(line,i,word); /*getting next word in line*/
				/*printf("\n\n%s\n\n",word);*/
			}
			
			
			
			if(word[0] == '.')
			{
				ok = ok & analyze_data(data,line,word,line_number,label_flag,symbol,&DC);
			}
			else
			ok = ok & analyze_cmd(command,line,word,line_number,label_flag,&IC);
			
				
			/*printf("\n\nin line %d ok in while: %d",line_number, ok);*/
			
			
		}		
/*-------------------------------------------------------------------------*/
				
	update_symbol_list(symbol,IC);
	update_data_list(data,IC);		
	print_symbol_list(symbol);
	print_command_list(command);
	print_data_list(data);

	free_command_list(command);
	free_data_list(data);
	free_symbol_list(symbol);
	
	printf("\n=======after free func========\n");	
	print_data_list(data);
	print_command_list(command);
	print_symbol_list(symbol);

	
	fclose(fd);
	printf("\n\nok is: %d\n",ok);
	printf("\nnumber of line is file is %d\nfinish file: %s\n",line_number,file_name); /*debug print*/
	
	printf("\n\n================================================\n\n");
}

/*copying first word from/after index i in 'from' array to 'to' array*/
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
int is_empty_line(char * line)
{
	if(line[0] == '\n')
		return 1;
	else
		return 0;
}

int is_label_def(char * word, int line_number, int * ok)
{
	int i = 0;
	if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
	{
		i++;
			
		while((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= '0' && word[i] <= '9'))
			{
				i++;
			}
			
		if(word[i] == ':' && word[i+1] == '\0')
		{
			/*checking if symbol is legal length*/
			if(strlen(word) <= SYMBOL_MAX_LEN)
				return 1;
			else
			{
				printf("\nERROR (line %d): symbol definitiom is too long", line_number);
				*ok = 0;
				return 0;
			}
		}	
			return 0;	
	}
	return 0;
}

int is_label(char * word)
{
	int i = 0;
	if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
	{
		i++;
			
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



int analyze_label_type(symbol_struct *symbol, char * line, char * label, int line_number, int DC, int IC)
{
	int i = 0;
	char word[MAX_WORD];
	
		/*checking if label isn't a command\register name*/
		if (label_check(label) == 0) 
		{
			printf("\nERROR (line %d): symbol name is a defined command/register name",line_number);
				return 0;
		}

	/*skipping to next char after ':'*/
		while(line[i] != ':')
		{
			i++;
		}
		i++;
		
		get_word(line,i,word); /*getting next word in line to determine type of label*/
		
		if(strcmp(word,".data") == 0)
		{
		
			if(insert_symbol(symbol,label,DC,DATA_SYMBOLKIND))
			return 1;
			else
			{
				printf("\nERROR (line %d): symbol already exists in list",line_number);
				return 0;
			}
		}
			
		if(strcmp(word,".string") == 0)
		{
			if(insert_symbol(symbol,label,DC,DATA_SYMBOLKIND))
			return 1;
			else
			{
				printf("\nERROR (line %d): symbol already exists in list",line_number);
				return 0;
			}
		}
			
		if(strcmp(word,".extern") == 0)
		{
			printf("\nWARNING (line %d): .extern command apears after label definition", line_number);
			return 1;
		}
		
		if(strcmp(word,".enrty") == 0)
		{
			printf("\nWARNING (line %d): .entry command apears after label definition", line_number);
			return 1;
		}
			
		/*in any other case it is a code label line*/
		if(insert_symbol(symbol,label,IC,CODE_SYMBOLKIND))
		return 1;
		else
		{
			printf("\nERROR (line %d): symbol already exists in list",line_number);
			return 0;
		}
}


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
	
	/*if is entry label line*/
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
				if(insert_symbol(symbol,word,0,EXERNAL_SYMBOLKIND))
					return 1;
				
				else
				{
				   printf("\nERROR (line %d): symbol already exists as local symbol",line_number);
				   return 0;
				}
			}
			else
			{
				printf("\nERROR (line %d): symbol name is a defined command/register name",line_number);
				return 0;
			}
		}
		printf("\nERROR (line %d): not a legal symbol name", line_number);
		return 0;
	}
	
	if(strcmp(word,".string") == 0)
	{
		if(analyze_string_cmd(data,line,label_flag,line_number,DC))
			return 1;
		else
		{
			return 0;
		}
	}
		
		if(strcmp(word,".data") == 0)
		{
			if(analyze_data_cmd(data,line,label_flag,line_number,DC))
				return 1;
			else
				return 0;
		}
		
		printf("\nERROR (line %d): unknown command", line_number);
		return 0;
		
	}


int analyze_cmd(command_struct * command, char * line, char * word, int line_number,int label_flag, int * IC)
{
int i =0;
CommandInfo* commandInfo;
command_struct* temp = command;

commandInfo = is_cmd(word);
if(commandInfo == NULL )
	{
		printf("\nERROR (line %d): '%s' is unknown command", line_number, word);
		return 0;
	}

if(label_flag)
	{
		while(line[i] != ':')
		{
			i++;
		}
			
			i++; /*to get ot char agter ':'*/
	}

while(spaceOrTab(line[i])) i++;

i = i + strlen(word);

while(temp->next!=NULL)
	temp=temp->next;
		
if(!(fill_arguments(line_number, (line + i), temp)))
	return 0;



insert_command(command,line,commandInfo,temp->arguments_num,&IC,line_number,temp->arguments);
(*IC) += get_command_size(temp);
return 1;
}

/*get the size that the command take(lines)*/
int get_command_size(command_struct* cur)
{
	int size = 0;
	int i;
	if (cur->arguments_num == 0)
	{
		return 1;
	}
	if (cur->arguments_num == 1)
	{	
		size+=2;
		switch(cur->arguments[0].addressingMode)
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
	if (cur->arguments_num == 2)
	{
		size+=2;
		for(i=0; i < cur->arguments_num; i++)
			{
				switch(cur->arguments[i].addressingMode)
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

CommandInfo* is_cmd(char *word)
{
int i;

for(i=0; i<=15; i++)
	{
		if(strcmp(word,commandInfos[i].commandName)==0)
			return &commandInfos[i];
	}
return NULL;
}

/*arrange the argument from the line, check for errors*/	
int fill_arguments(int line_number, char* line, command_struct* command)
{
	int i = 0, j = 0, k;


	while (line[i])
	{
		while ((line[i] != '\0') && spaceOrTab(line[i]))
			i++;
		if (line[i] == '\0')
			break;
		if (line[i] == ',')
		{
			if (j == 0)
			{
				printf("Extra comma before arguments, in line number %d",line_number);
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
		while ((line[i] != '\0') && !spaceOrTab(line[i]) && (line[i] != ','))
		{
			command->arguments[j].argument_str[k] = line[i];
			k++;
			i++;
		}
		command->arguments[j].argument_str[k] = '\0';
		if (strlen(command->arguments[j].argument_str) == 0)
		{
			printf("Empty argument. in line %d",line_number);
			return 0;
		}
		

		if (!fill_addressing_mode(&command->arguments[j]))
		{
			printf("Bad addressing mode for argument. in line %d",line_number);
			return 0;
		}
		j++;
	}
	if (j > 0)
	{
		command->arguments_num = j;
	}
	return 1;

}	

/*fill the adrresing mode each command contain*/
int fill_addressing_mode(argument_struct* argument)
{
	if (fill_immediete_addressing_mode(argument))
	{
		return 1;
	}
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
	return 0;
}

int fill_immediete_addressing_mode(argument_struct* argument)
{
	int succeded;
	if (strlen(argument->argument_str) < 2)
		return 0;
	if (argument->argument_str[0] != '#')
		return 0;
	/*delete the # from the begin of the line*/
	move_left(argument->argument_str, 1);
	/* not use the returned value because this is just a check
	if there is a number in argument */
	get_number_from_string(argument->argument_str, &succeded);
	if (succeded)
		argument->addressingMode = IMMEDIETE;
	return succeded;
}

/*TODO check that we dont creat more words here */
int fill_register_addressing_mode(argument_struct* argument)
{
	int num, succeded;
	if ((strlen(argument->argument_str) != 2) && (strlen(argument->argument_str) != 3))
		return 0;
	if (argument->argument_str[0] != 'r')
		return 0;
	num = get_number_from_string(&argument->argument_str[1], &succeded);
	if (!succeded)
		return 0;
	if ((num < 0) || (num > 15))
		return 0;
	/*clean and leave just the number of the argument*/
	move_left(argument->argument_str, 1);
	argument->addressingMode = REGISTER;
	return 1;
}



int fill_direct_addressing_mode(argument_struct* argument)
{
	if (!is_label(argument->argument_str))
		return 0;
	argument->addressingMode = DIRECT;
	return 1;
}

int fill_index_addressing_mode(argument_struct* argument)
{
	
	if (strlen(argument->argument_str) < 6)
		return 0;
		
	if( !(symbol_and_register_is_ligal(argument->argument_str)))
		return 0;
	
	argument->addressingMode = INDEX;
	return 1;
}



int symbol_and_register_is_ligal(char* word)
{
	int i=0;
	int j=0;
	int num;
	char symbol_str[SYMBOL_MAX_LEN];
	char register_str[MAX_REGISTER_LEN];

	while(word[i] != '\0' && word[i] != '[')
		symbol_str[j++]=word[i++];
	
	symbol_str[j]='\0';

	if(!(is_label(symbol_str)))
		return 0;
	j=0;
	
	if(word[i] != '[')
		return 0;
	i++;

	if(word[i] != 'r')
		return 0; 
	i++;

	while( word[i] != ']' && word[i] != '\0')
		register_str[j++] = word[i++];
	register_str[j]='\0';

	if(word[i]!=']')
		return 0;	
	num = atoi(register_str);

	if ((num < 10) || (num > 15))
		return 0;
	
	return 1;
}

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
		printf("\nERROR (line %d): missing string in line", line_number);
		return 0;
	}
		
	if (line[i] != '"')
	{
		printf("\nERROR (line %d): missing quoation mark in string", line_number);
		return 0;
	}
	
	i++;
	
	while ((line[i] != '\0') && (line[i] != '"'))
	{
		str[j++] = line[i++];
		count++;
	}
	str[j] = '\0';
	count++;
		
	if (line[i] != '"')
	{
		printf("\nERROR (line %d): missing quoation mark in string", line_number);
		return 0;
	}
	
	i++;
	while ((line[i] != '\0') && spaceOrTab(line[i]))
		i++;
	if (line[i] != '\0')
	{
		printf("\nERROR (line %d): extra character after string", line_number);
		return 0;
	}
	
	if(insert_data(data,line,str,NULL,count,*DC,STRING_DATAKIND))
	{
		*DC += strlen(str)+1;
		return 1;
	}
	else
	{
		printf("\nERROR (line %d): failed to allocate data", line_number);
		return 0;
	}
}

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
	
	if(line[i] == '\n')
	{
		printf("\nERROR (line %d): missing data in line", line_number);
		return 0;
	}
	
	if(fill_numbers(line,i,values,line_number,&count))
	{
		
		if(insert_data(data,line,NULL,values,count,*DC,DATA_DATAKIND))
		{
			*DC += count;
			return 1;
		}
		else
		{
			printf("\nERROR (line %d): failed to allocate data", line_number);
			return 0;
		}
	}
	return 0;
}
	
/*this function will recieve the .data line and extract the numbers inti an int array
will return 1 if successful, 0 if not*/
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
			printf("\nERROR (line %d): ilegal use of comma", line_number);
			return 0;
		}
			
		while (line[i] && spaceOrTab(line[i]))
			i++;
			
		while (line[i] && (line[i] != ',') && !spaceOrTab(line[i])) /*potential number*/
			num_str[j++] = line[i++];
		
		while (line[i] && spaceOrTab(line[i]))
			i++;
				
		if (line[i] && (line[i] == ','))
		{
			i++;
			
			while (line[i] && spaceOrTab(line[i]))
				i++;
				
			if(!line[i]) 
			{
				printf("\nERROR (line %d): ilegal comma at end of data", line_number); /*needs to be number after comma*/
				return 0;
			}
		}
		else if(!line[i])
		{
			; /*do nothing*/
		}
		else 
		{
			printf("\nERROR (line %d): missing comma", line_number);
			return 0;
		}
		
		
			
				
		if(line[i] == ',') /*double comma*/
		{
			printf("\nERROR (line %d): ilegal use of comma", line_number);
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
			printf("\nERROR (line %d): one of the data parametes is not a number", line_number);
			return 0;
		}	
	}
		return 1;
}










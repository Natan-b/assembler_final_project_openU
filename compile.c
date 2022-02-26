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


int IC = 100;
int DC = 0;
int ok = 1;

/* a "table" of the info of each optional command */
CommandInfo commandInfos[] =
{
	{MOV, "mov", 0, 0, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{CMP, "cmp", 1, 0, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{3, {IMMEDIETE, DIRECT, REGISTER}, TARGET}}},
	{ADD, "add", 2, 10, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{SUB, "sub", 2, 11, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{LEA, "lea", 4, 0, 2,
		{{1, {DIRECT}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{CLR, "clr", 5, 10, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{NOT, "not", 5, 11, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{INC, "inc", 5, 12, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{DEC, "dec", 5, 13, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{JMP, "jmp", 9, 10, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{BNE, "bne", 9, 11, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{JSR, "jsr", 9, 12, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{RED, "red", 12, 0, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{PRN, "prn", 13, 0, 1,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, TARGET}}},
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
	while(!feof(fd))
		{
			i=0;
			label_flag = 0;		
			/* get new line from the file */
			fgets(full_line,MAX_ONE_LINE,fd);
			line_number++;
			/* remove extra whitespaces and tabs from beginnig of string */
			clean_line(full_line,line);
			
			/*checking if line is a comment line*/
			if(is_comment(line))
			continue; /*will skip to the next line*/
			
			/*checking if line is an empty line*/
			if(is_empty_line(line))
			continue; /*will skip to the next line*/
			
			get_word(line,i,word); /*recieving word*/
			
			/*checking if first word is a label definition or external label definition*/
			if(is_label_def(word,line_number))
			{
				if(analyze_label_type(symbol,line,word, line_number))
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
				ok = ok && analyze_data(line,word,line_number,label_flag,symbol);
			}
			
			ok = ok && analyze_cmd(command,line,word,line_number,label_flag);
			
				
			
			
			
		}		
/*-------------------------------------------------------------------------*/
				
			
	print_symbol_list(symbol);
	print_command_list(command);

	/*free_symbol_list(symbol);*/
	free_command_list(command);
	free_data_list(data);
	
	printf("\n=======after free func========\n");
	
	/*print_symbol_list(symbol);*/
	print_command_list(command);	
	print_data_list(data);
	
	

	
	fclose(fd);

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

int is_label_def(char * word, int line_number)
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
				ok = 0;
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



int analyze_label_type(symbol_struct *symbol, char * line, char * label, int line_number)
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
			
		/*in any other case it is a code label line*/
		if(insert_symbol(symbol,label,IC,CODE_SYMBOLKIND))
		return 1;
		else
		{
			printf("\nERROR (line %d): symbol already exists in list",line_number);
			return 0;
		}
}


int analyze_data(char * line, char * word, int line_number,int label_flag,symbol_struct *symbol)
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
	
	
	return 1;
}


int analyze_cmd(command_struct * command, char * line, char * word, int line_number,int label_flag)
{
int i =0;
CommandInfo* commandInfo;	
	if(label_flag)
	{
		while(line[i] != ':')
		{
			i++;
		}
			
			i++; /*to get ot char agter ':'*/
		commandInfo = is_cmd(word);
		if(commandInfo == NULL )
			{
				printf("\n==========the command %s not found==========\n",word);
				return 0;
			}
		/*TODO send the right argument of the argument number */
		/*TODO take care of the IC counter*/

		insert_command(command,line,commandInfo,2,IC,line_number);


/*printf("==========\n%s\n%s\n%d\n%d\n=======",line,word,line_number,label_flag);*/
		
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

	
	














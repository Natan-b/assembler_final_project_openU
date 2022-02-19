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
symbol_struct * cs;

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
/*char c;*/
int i = 0;

FILE * fd;
symbol_struct *cs = create_symbol_struct();

sprintf(preprocess_file_name,"%s.am",file_name);
fd = fopen(preprocess_file_name,"r");

/*opening processed file*/
/*fd = fopen("k1","r");*/

	if(fd == NULL)
	{
		printf("\ncannot open file %s\n", file_name);
		return;
	}

	while(!feof(fd))
		{
			/* get new line from the file */
			fgets(full_line,MAX_ONE_LINE,fd);
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
			if(is_label_def(word) || (is_external_def(word)))
				analyze_label_line(cs,line,word);
				
/*-------------------------------------------------------------------------*/
				
		}	
	print_symbol_list(cs);
	printf("\n\ncompile file works\n\n");
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

int is_label_def(char * word)
{
	int i = 0;
	if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
	{
		i++;
			
		while((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= '0' && word[i] <= '9'))
			{
				i++;
			}
			
		if(word[i] == ':')
		{
			if(word[i+1] == '\0')
			return 1;
		}	
		else
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
			return 1;
		}	
		else
			return 0;
			
	}
	return 0;
}

int is_external_def(char * word)
{
	if(strcmp(word,".extern") == 0)
		return 1;
	else
		return 0;
}

void analyze_label_line(symbol_struct *cs, char * line, char * word)
{
	int i = 0;

	/*if line is external label definition*/
	if(is_external_def(word))
	{
		while(line[i] != 'n') /*skipping to char after word ".extern"*/
		{
			i++;
		}
		i++;
		
		/*receiving next word*/
		get_word(line,i,word); 
		/*checking to see if word after ".extern" is a label sign*/
		if(is_label(word))
		{
			insert_symbol(cs,word,0,2);
			return;
		}
		else
		printf("\n\nERROR - word isn't legal label\n\n");
	}
	
	insert_symbol(cs,word,IC,1);
}


/*NEED TO TAKE CARE OF SYMBAL ATRIBUTES
	FIND OUT WHY HE DID STRUCT OF BIT FIELDS FOR STRIBUTES
		FREE SYMBOL LIST FUNCTIOIN
*/
	
	














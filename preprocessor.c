#include "preprocessor.h"
#include "general_functions.h"
#include "hash.h"
#include "struct.h"
#include "constants.h"
#include "compile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/* get the file name one after the other*/
void get_files(int number_of_files, char *file_names[])
{
	int i=1;

	while((number_of_files--)>1)
			check_file(file_names[i++]);

		
}


/* the func gets the .as name file and creat the .am file after layout the macro names in the .am file */
void check_file(char *file_name)
{
	int i;
	int macroFlag= NOT_FIND;
	int hash_table_creat_flag = NO;
	int index;
	char line[MAX_ONE_LINE];
	char clear_line[MAX_ONE_LINE];
	char macro_name[MAX_WORD];

	char asembler_file_name[MAX_NAME_FILE];
	char preprocess_file_name[MAX_NAME_FILE];
	
	FILE *asembler_file, *preprocess_file; 

	macro_name_node *cur;
	
	if(strlen(file_name) <= MAX_NAME_FILE)
	{

	macro_name_node ** hash_table= malloc(HASHSIZE * sizeof(macro_name_node *));;
	
	for(i=0;i<HASHSIZE;i++)
		hash_table[i]=NULL;
	
		

	sprintf(asembler_file_name,"%s.as",file_name);
	asembler_file = fopen(asembler_file_name,"r");

	if(asembler_file== NULL)
		{
			printf("\ncannot open file %s\n", asembler_file_name);
			return;
		}

	sprintf(preprocess_file_name,"%s.am",file_name);	
	preprocess_file = fopen(preprocess_file_name,"w");

	if(preprocess_file== NULL)
		{
			printf("\ncannot open file %s\n", preprocess_file_name);
			return;
		}

	printf("\nStart processing assembler file '%s'\n",file_name);

	while(1)
		{	
			/* get new line from the file */
			fgets(line,MAX_ONE_LINE,asembler_file);
			if(feof(asembler_file)) break;
			/* remove extra whitespaces and tabs */
			clean_line(line,clear_line);

			if(strlen(line) == 0 )
			continue; /*will skip to the next line*/
			/* check if line have macro*/
			macroFlag=check_line(clear_line,macro_name,"macro");
			
			/* get macro lines*/
			if(macroFlag==FIND)
				{	
					if(hash_table_creat_flag==NO)
							hash_table_creat_flag=YES;
										
					macroFlag=NOT_FIND;
					while(macroFlag==NOT_FIND)
						{
							/* get new line from the file */
							fgets(line,MAX_ONE_LINE,asembler_file);
							/* remove extra whitespaces and tabs */
							clean_line(line,clear_line);
							macroFlag=check_line(clear_line,NULL,"endm");
							if(macroFlag==NOT_FIND)
								{
									/* creat linked list for macro */
									hash_table_insert(hash_table,macro_name,clear_line);
								}						
						}
				}
			else
				{
					if(hash_table == NULL) 
						{
							/* no macro found in .as file*/
							fputs(clear_line,preprocess_file);
							fputs("\n",preprocess_file);
						}
					else
						{
							/* get the index macro is the hash table */
							index = hash(clear_line);
							/*  check if it is a macro line in the hash table*/
							if(hash_table[index] != NULL) 
								{
									cur = hash_table[index];
									
									/* write the macro lines to the .am file */
									while(cur != NULL)
										{
											fputs(cur->macro_line,preprocess_file);
											fputs("\n",preprocess_file);
											cur = cur->next;
										}
									
								}				
							else
								{
									fputs(clear_line,preprocess_file);
									fputs("\n",preprocess_file);
								}
						}

				}
		}
	/* free the hash table if created*/
	if(hash_table_creat_flag==YES)
			free_hash_table(hash_table);
	free(*hash_table);		

	fclose(asembler_file);
	fclose(preprocess_file);
	/*send to compile.c for compilation*/
	compile_file(file_name); 
	}
	else
		printf("\nERROR : file name '%s' is too long\n", file_name);
}


/* generic func that check if the word_check is in the line */
int check_line(char *line, char *macro_name, char * word_check)
{
	int i,j;
	char word[MAX_WORD];
	i=0;
	j=0;
	/* get first words in line */
	while(line[i]!=' ' && line[i] !='\t')
		word[j++]=line[i++];
	word[j]='\0';

	if(strcmp(word,word_check)==0)
		{
			if(macro_name==NULL)
				return FIND;
			/* get check word*/
			i++;
			j=0;
			while(line[i]!='\0' && line[i]!='\n')
				macro_name[j++]=line[i++];
			macro_name[j]='\0';
			
			return FIND;
		}
	else
		return NOT_FIND;
}






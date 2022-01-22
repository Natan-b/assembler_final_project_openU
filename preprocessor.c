#include "compile.h"

/* add free nod list and char words */



void get_files(int number_of_files, char *file_names[])
{
	int i=1;

	printf("number of files %d\n", number_of_files-1);
	while((number_of_files--)>1)
			check_file(file_names[i++]);

		
}

void check_file(char *file_name)
{
	int macroFlag= NOT_FIND;
	int index;
	char line[MAX_ONE_LINE];
	char clear_line[MAX_ONE_LINE];
	char macro_name[MAX_WORD];

	char asembler_file_name[MAX_NAME_FILE];
	char preprocess_file_name[MAX_NAME_FILE];
	
	FILE *asembler_file, *preprocess_file; 

	macro_name_node ** hash_table;
	macro_name_node *cur;
	hash_table = NULL;
	/* reset hash tab to be empty */
	/*init_hash_table();*/
	
	
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
			printf("\ncannot open file %s\n", asembler_file_name);
			return;
		}

	while(!feof(asembler_file))
		{
			/* get new line from the file */
			fgets(line,MAX_ONE_LINE,asembler_file);
			/* remove extra whitespaces and tabs */
			clean_line(line,clear_line);
			/* check if line have macro*/
			macroFlag=check_line(clear_line,macro_name,"macro");
			/* get macro lines*/
			if(macroFlag==FIND)
				{	
					hash_table = creat_hash_table();					
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
					if(hash_table == NULL) /* no macro found */
						{
							fputs(clear_line,preprocess_file);
							fputs("\n",preprocess_file);
						}
					else
						{
							index = hash(clear_line);
							printf("%d %s\n",index,clear_line);
							if(hash_table[index] != NULL) /* it is a macro line */
								{
									cur = hash_table[index];
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
	print_table(hash_table);
	fclose(asembler_file);
	fclose(preprocess_file);
}

int check_line(char *line, char *macro_name, char * word_check)
{
	int i,j;
	char word[MAX_WORD];
	i=0;
	j=0;
	/* get first words in line */
	while(line[i]!=' ')
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

int spaceOrTab(char c)
{
	return (c == ' ') || (c == '\t');
}

void clean_line(char * s, char * d)
{

	int i = 0, j = 0;
	while ((s[i] != '\0') && spaceOrTab(s[i]))
	{
		i++;
	}
	while ((s[i] != '\0') && (s[i] != '\n'))
	{
		d[j++] = s[i++];
	}
	d[j] = '\0';
}


unsigned int hash(char * macro_name)
{
	int length= strlen(macro_name);
	int i;	
	unsigned int hash_value = 0;
	for(i=0;i<length;i++)
		{
			hash_value+=macro_name[i];
			hash_value = (hash_value * macro_name[i] % HASHSIZE);
		}
	return hash_value;
}

/*void init_hash_table()
{
	int i;
	for(i=0;i<HASHSIZE;i++)
		hashtab[i]=NULL;
} */

void print_table(macro_name_node ** hash_table)
{
	macro_name_node *head;
	macro_name_node *cur;
	int i;
	for(i=0;i<HASHSIZE;i++)
		{
			head = hash_table[i];
		
			printf("%d: ",i);

			if(head==NULL)
				printf("NULL");
			else
				{
					cur = head;
					while(cur != NULL)
						{
							printf("\n%s\n", cur->macro_line);
							cur = cur->next;
						}
				}
			printf("\n");
		}	
}



void hash_table_insert(macro_name_node ** hash_table,char * macro_name, char * macro_line)
{
	macro_name_node *head;
	unsigned int hash_value;
	
	hash_value = hash(macro_name);
	
	if(hash_table[hash_value] == NULL)
		{
			hash_table[hash_value] = malloc(sizeof (macro_name_node *));
			
			head = NULL;
			insert_new_line(&head , macro_line);

			hash_table[hash_value]= head;
		}

	else
		{
			head = hash_table[hash_value];
			insert_new_line(&head , macro_line);
		
			hash_table[hash_value]=head;
		}
}

void insert_new_line(macro_name_node ** head,char * new_line)
{
	macro_name_node * cur = *head;	
	macro_name_node * new_macro_line = (macro_name_node*)malloc(sizeof(macro_name_node));
	strcpy(new_macro_line->macro_line,new_line);
	new_macro_line->next = NULL;
	if(*head == NULL)
		*head = new_macro_line;

	else 
		{
			while(cur->next !=NULL)
				cur = cur->next;
			cur->next = new_macro_line;
		}

}


macro_name_node ** creat_hash_table()
{
	macro_name_node ** hash_table = malloc(HASHSIZE * sizeof(macro_name_node *));
	return	hash_table;
}


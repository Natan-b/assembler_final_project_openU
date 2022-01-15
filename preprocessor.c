#include "compile.h"

void get_files(int number_of_files, char *file_names[])
{
	int i=1;

	printf("number of files %d\n", number_of_files);
	while((number_of_files--)>1)
			check_file(file_names[i++]);

		
}

void check_file(char *file_name)
{
	char buf[MAX_ONE_LINE];
	char asembler_file_name[MAX_NAME_FILE];
	char preprocess_file_name[MAX_NAME_FILE];

	FILE *asembler_file, *preprocess_file; 
	
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
			fgets(buf,MAX_ONE_LINE,asembler_file);
			fputs(buf,preprocess_file);
		}

	fclose(asembler_file);
	fclose(preprocess_file);
}

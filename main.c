#include "compile.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
		{
			printf("Sory, no file found\nPlease enter a name of file\n");
			return 0;
		}
	get_files(argc, argv);
	return 0;
}

#include "general_functions.h"
#include <stdio.h>



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
<<<<<<< HEAD
	while ((s[i] != '\0') && (s[i] != '\n'))
=======
	while (s[i] != '\0')
>>>>>>> f7375110181730651ad2cfa6d0b1b039311907c2
	{
		d[j++] = s[i++];
	}
	d[j] = '\0';
}



/* return 1 if c is upper case letter */
int is_upper_case(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return 1;
	else
		return 0;
}



/* return 1 if c is lower case letter */
int is_lower_case(char c)
{
	if ((c >= 'a') && (c <= 'z'))
		return 1;
	else
		return 0;
}



/* return 1 if c is a letter */
int is_letter(char c)
{
	return is_upper_case(c) || is_lower_case(c);
}



int is_number(char c)
{
	if ((c >= '0') && (c <= '9'))
		return 1;
	else
		return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> f7375110181730651ad2cfa6d0b1b039311907c2

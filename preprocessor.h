#ifndef COMPILE_H
#define COMPLIE_H


/* get the file name one after the other*/
void get_files(int , char **);
/* the func gets the .as name file and creat the .am file after layout the macro names in the .am file */
void check_file(char *);
/* generic func that check if the word_check is in the line */
int check_line(char *,char*, char *);



#endif

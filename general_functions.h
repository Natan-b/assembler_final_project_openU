#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

/*function skips space and tab chars*/
int spaceOrTab(char c);

/*clean from space at the begin and at the end and copy from src to dst*/
void clean_line(char* src, char* dst);

/*function checks if char is letter*/
int is_letter(char c);

/*function checks if char is number*/
int is_number(char c);

/*function moves string indexes left num times*/
void move_left(char* str, int num);

/*function extract number from string*/
int get_number_from_string(char* num_str, int* succeded);


#endif

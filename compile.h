#ifndef COMPILE_H
#define COMPILE_H

/*starts checking file after being processed*/
void compile_file(char * file_name);
void get_word(char *,int i, char *);
int is_comment(char *);
int is_empty_line(char *);
int is_label(char * word);
int is_label_def(char * word, int* line_number);
int is_external_def(char * word);
void analyze_label_type(symbol_struct *cs, char * line, char * word, int * line_number);
int analyze_data(char * line, char * word, int line_number,int label_flag);
int analyze_cmd(command_struct *, char * line, char * word, int  line_number, int label_flag);
CommandInfo* is_cmd(char *word);




#endif

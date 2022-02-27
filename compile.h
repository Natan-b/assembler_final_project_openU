#ifndef COMPILE_H
#define COMPILE_H

/*starts checking file after being processed*/
void compile_file(char * file_name);
void get_word(char *,int i, char *);
int is_comment(command_struct *,char *, char *,int,int * );
int is_empty_line(char *);
int is_label(char * word);
int is_label_def(char * word, int line_number);
int analyze_label_type(symbol_struct *cs, char * line, char * word, int  line_number, int DC, int * IC);
int analyze_data(char * line, char * word, int line_number,int label_flag, symbol_struct * symbol);
int analyze_cmd(command_struct *, char * line, char * word, int  line_number, int label_flag, int * IC);
CommandInfo* is_cmd(char *word);
int fill_arguments(int line_number, char* line, command_struct* command);
int fill_addressing_mode(argument_struct* argument);
int fill_immediete_addressing_mode(argument_struct* argument);
int fill_register_addressing_mode(argument_struct* argument);
int fill_direct_addressing_mode(argument_struct* argument);
int fill_relative_addressing_mode(argument_struct* argument);
int symbol_is_legal(char* name);




#endif

#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <ctype.h>

/*starts checking file after being processed*/
void compile_file(char * file_name);
void get_word(char *,int i, char *);
int is_comment(char * line);
int is_empty_line(char *, int i);
int is_label(char * word);
int is_label_def(char * word, int line_number, int * ok);
int analyze_label_type(symbol_struct *cs, char * line, char * word, int  line_number, int DC, int IC);
int analyze_data(data_struct * data ,char * line, char * word, int line_number,int label_flag, symbol_struct * symbol, int * DC);
int analyze_cmd(command_struct *, char * line, char * word, int  line_number, int label_flag, int * IC);
CommandInfo* is_cmd(char *word);
int fill_arguments(int line_number, char* line, command_struct* command);
int fill_addressing_mode(argument_struct* argument);
int fill_immediete_addressing_mode(argument_struct* argument);
int fill_register_addressing_mode(argument_struct* argument);
int fill_direct_addressing_mode(argument_struct* argument);
int fill_index_addressing_mode(argument_struct* argument);
int analyze_string_cmd(data_struct * data,char * line,int label_flag, int line_number, int * DC);
int analyze_data_cmd(data_struct * data, char * line, int label_flag,int line_number, int * DC);
int fill_numbers(char * line, int i, int * values, int line_number, int * count);
int label_check(char * label);
int symbol_and_register_is_ligal(char* word);
void get_symbol_and_register(char* word, char* register_str, char* symbol_str);
int get_command_size(command_struct* command);
void write_ob_file(FILE* ob_file, command_struct * command,data_struct * data, symbol_struct * symbol);
void write_command_to_ob_file(FILE* ob_file, command_struct* command, symbol_struct* symbol);
void write_word(FILE* file, int address, unsigned int word);
symbol_struct* find_symbol(char* name, symbol_struct * symbol);
int check_memory(data_struct* data, command_struct* command);
int check_command_symbols (command_struct * command, symbol_struct * symbol, int line_number);
void write_ent_file(FILE* ent_file, symbol_struct * symbol);
void write_ext_file(FILE* ext_file, command_struct * command, symbol_struct * symbol);
void write_data_to_ob_file(FILE* ob_file, data_struct* cur_data);



#endif

#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <ctype.h>

/*starts checking file after being processed*/
void compile_file(char * file_name);

/*this function will copy first word from/after index i in 'from' array to 'to' array*/
void get_word(char *,int i, char *);

/*function will check if line is a comment line*/
int is_comment(char * line);

/*function will check if line is an empty line*/
int is_empty_line(char *, int i);

/*this function will check to see if the given word is a label*/
int is_label(char * word);

/*this function will check to see if the given word is a label definition*/
int is_label_def(char * word, int line_number, int * ok);

/*function analyzes the type of label before inserting into symbol list*/
int analyze_label_type(symbol_struct *cs, char * line, char * word, int  line_number, int DC, int IC);

/*function analyzes possible data line and passes on for further inspection*/
int analyze_data(data_struct * data ,char * line, char * word, int line_number,int label_flag, symbol_struct * symbol, int * DC);

/*function analyzes possible command line and passes on for further inspection*/
int analyze_cmd(command_struct *, char * line, char * word, int  line_number, int label_flag, int * IC);

/*function checks if given word is a command name and return the command's information*/
CommandInfo* is_cmd(char *word);

/*function arranges the arguments from the line and checks for errors*/	
int fill_arguments(int line_number, char* line, command_struct* command);

/*function fills the adrresing mode each command line arguments*/
int fill_addressing_mode(argument_struct* argument);

/*function checks if arguments in line are of immediate addressing mode*/
int fill_immediete_addressing_mode(argument_struct* argument);

/*function checks if arguments in line are of register addressing mode*/
int fill_register_addressing_mode(argument_struct* argument);

/*function checks if arguments in line are of direct addressing mode*/
int fill_direct_addressing_mode(argument_struct* argument);

/*function checks if arguments in line are of index addressing mode*/
int fill_index_addressing_mode(argument_struct* argument);

/*function analyzes string data command before inserting into data list*/
int analyze_string_cmd(data_struct * data,char * line,int label_flag, int line_number, int * DC);

/*function analyzes data data command before inserting into data list*/
int analyze_data_cmd(data_struct * data, char * line, int label_flag,int line_number, int * DC);

/*this function will recieve the .data line and extract the numbers into an int array*/
int fill_numbers(char * line, int i, int * values, int line_number, int * count);

/*function checks to see if given label is not a register\command name*/
int label_check(char * label);

/*function checks if symbol and register are legal in direct addressing mode format*/
int symbol_and_register_is_ligal(char* word);

/*function gets the symbol and register from line (in direct addressing mode format line)*/
void get_symbol_and_register(char* word, char* register_str, char* symbol_str);

/*function calculates the size in "words" that the given command takes*/
int get_command_size(command_struct* command);

/*function creates the ob file and calls the print to file methode*/
void write_ob_file(FILE* ob_file, command_struct * command,data_struct * data, symbol_struct * symbol);

/*function arranges the command and arguments as wish and calls the print method*/
void write_command_to_ob_file(FILE* ob_file, command_struct* command, symbol_struct* symbol);

/*function writes word to the ob file */
void write_word(FILE* file, int address, unsigned int word);

/*function finds symbol name in the list of symbols and return address*/
symbol_struct* find_symbol(char* name, symbol_struct * symbol);

/*function checks if given file exceeeds memeroy capacity in computer*/
int check_memory(data_struct* data, command_struct* command);

/*this function will check if potentail symbold in command line exist in symbol list*/
int check_command_symbols (command_struct * command, symbol_struct * symbol, int line_number);

/*function creates and writes the ent file*/
void write_ent_file(FILE* ent_file, symbol_struct * symbol);

/*function creates and writes the ext file*/
void write_ext_file(FILE* ext_file, command_struct * command, symbol_struct * symbol);

/*function translates data to ob format and writes it in file*/
void write_data_to_ob_file(FILE* ob_file, data_struct* cur_data);



#endif

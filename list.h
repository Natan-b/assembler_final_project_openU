#ifndef LIST_H
#define LIST_H

/*function adds new symbol node to symbol list*/
int insert_symbol(symbol_struct * cs,char * s_name, int s_value, int atr);

/*function inserts command node to list*/
void insert_command(command_struct * ,char * ,CommandInfo*, int, int**,int , struct argument_struct * arguments);

/*function inserts data node to list*/
int insert_data(data_struct * data, char * label, char * str, int * values, int values_num, int DC, DataKind kind);

/*function frees symbol list*/
void free_symbol_list(symbol_struct * head);

/*function frees command list*/
void free_command_list(command_struct *);

/*function frees data list*/
void free_data_list(data_struct *);

/*function updates addresses on symbol list*/
void update_symbol_list(symbol_struct * symbol, int address);

/*function updates addresses on data list*/
void update_data_list(data_struct * data, int address);

/*function gets data list size in "words"*/
int get_data_size(data_struct * data);

/*function updates symbol list by confirming and updating symbol entries in file*/
int update_symbol_entry(symbol_struct * symbol,char * word, int line_number);
#endif

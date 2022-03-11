#ifndef LIST_H
#define LIST_H

/*adds new symbol to symbol list*/
int insert_symbol(symbol_struct * cs,char * s_name, int s_value, int atr);
void insert_command(command_struct * ,char * ,CommandInfo*, int, int**,int , struct argument_struct * arguments);
int insert_data(data_struct * data, char * label, char * str, int * values, int values_num, int DC, DataKind kind);
void print_symbol_list(symbol_struct * cs);
void print_command_list(command_struct *);/*debug printing*/
void free_symbol_list(symbol_struct * head);
void print_data_list(data_struct *);/*debug printing*/
void free_command_list(command_struct *);
void free_data_list(data_struct *);
void update_symbol_list(symbol_struct * symbol, int address);
void update_data_list(data_struct * data, int address);
int get_data_size(data_struct * data);
#endif

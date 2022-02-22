#ifndef LIST_H
#define LIST_H

/*adds new symbol to symbol list*/
void insert_symbol(symbol_struct * cs,char * s_name, int s_value, int atr);
void print_symbol_list(symbol_struct * cs);
void print_command_list(command_struct *);/*debug printing*/
void print_data_list(data_struct *);/*debug printing*/
void free_command_list(command_struct *);
void free_data_list(data_struct *);
#endif

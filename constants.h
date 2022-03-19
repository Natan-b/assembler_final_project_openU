#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_NAME_FILE 20 /* max file name length */
#define MAX_ONE_LINE 1000 /* max line length */
#define MAX_WORD 1000 /* max word length */
#define SYMBOL_MAX_LEN 31 /* max symbol length */
#define MAX_REGISTER_LEN 6 /* max register length */
#define MAX_ARGUMENT_LEN 100 /* max argument length */
#define MAX_ARGUMENTS_NUMBER 2 /* max number of arguments */
#define ADDRESSING_MODES_NUM 4 /*number of addressing modes*/
#define MAX_INTS_IN_DATA 40 /* max number of integers in .data */
#define THRESHOLD_IMMEDIATE_NUM 65535 /* max number for immediate is 16 bit wide = 65536*/
#define MAX_MEMORY_NUM 8191 /* max memory in computer*/


/*command types enum*/
typedef enum CommandType
{
	MOV,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	JSR,
	RED,
	PRN,
	RTS,
	STOP
} CommandType;

/*addressing modes enum*/
typedef enum AddressingMode
{
	IMMEDIETE,
	DIRECT,
	INDEX, 
	REGISTER
} AddressingMode;

/*argument kind enum*/
typedef enum ArgumentKind
{
	SOURCE,
	TARGET
} ArgumentKind;

/*symbol kind enum*/
typedef enum SymbolKind
{
	CODE_SYMBOLKIND,
	DATA_SYMBOLKIND,
	EXERNAL_SYMBOLKIND,
	CODE_ENTRY_SYMBOLKIND,
	DATA_ENTRY_SYMBOLKIND
} SymbolKind;

/*data kind enum*/
typedef enum DataKind
{
	STRING_DATAKIND = 1,
	DATA_DATAKIND = 2
} DataKind;


#endif

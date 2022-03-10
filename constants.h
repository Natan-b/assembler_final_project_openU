#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_NAME_FILE 20
/* max line length */
#define MAX_ONE_LINE 1000
#define MAX_WORD 40
#define SYMBOL_MAX_LEN 31
#define MAX_REGISTER_LEN 6
/* max argument length */
#define MAX_ARGUMENT_LEN 100
/* max number of arguments */
#define MAX_ARGUMENTS_NUMBER 2
/*number of addressing modes*/
#define ADDRESSING_MODES_NUM 4
/* max number of integers in .data */
#define MAX_INTS_IN_DATA 40




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
	INDEX, /*RELATIVE*/
	REGISTER
} AddressingMode;

/*argument kind enum*/
typedef enum ArgumentKind
{
	SOURCE,
	TARGET
} ArgumentKind;

typedef enum SymbolKind
{
	CODE_SYMBOLKIND,
	DATA_SYMBOLKIND,
	EXERNAL_SYMBOLKIND,
	CODE_ENTRY_SYMBOLKIND,
	DATA_ENTRY_SYMBOLKIND
} SymbolKind;

typedef enum DataKind
{
	STRING_DATAKIND = 1,
	DATA_DATAKIND = 2
} DataKind;


#endif

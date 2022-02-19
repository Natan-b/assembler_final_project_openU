#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_NAME_FILE 20
/* max line length */
#define MAX_ONE_LINE 80
#define MAX_WORD 25
#define SYMBOL_MAX_LEN 31
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
	RELATIVE,
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
	CODE_SYMBOLKIND = 1,
	DATA_SYMBOLKIND = CODE_SYMBOLKIND << 1,
	ENTRY_SYMBOLKIND = DATA_SYMBOLKIND << 1,
	EXERNAL_SYMBOLKIND = ENTRY_SYMBOLKIND << 1
} SymbolKind;


#endif

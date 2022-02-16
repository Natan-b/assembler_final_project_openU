#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_NAME_FILE 20
#define MAX_ONE_LINE 80
#define MAX_WORD 25
#define SYMBOL_MAX_LEN 31

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

typedef enum SymbolKind
{
	CODE_SYMBOLKIND = 1,
	DATA_SYMBOLKIND = CODE_SYMBOLKIND << 1,
	ENTRY_SYMBOLKIND = DATA_SYMBOLKIND << 1,
	EXERNAL_SYMBOLKIND = ENTRY_SYMBOLKIND << 1
} SymbolKind;


#endif

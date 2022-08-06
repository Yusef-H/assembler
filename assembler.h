#ifndef ASSEMBLER_H

#define ASSEMBLER_H

/* Bounded Sizes */
#define SIZE 1000
#define MAX_LENGTH 81 /* extra one for '\n' */
#define MAX_LABEL_LENGTH 30

/* constants */
#define NUM_DIRECTIVES 5
#define NUM_COMMANDS 16
#define NUM_REGISTERS 8


/* file types */
enum file_types{AS, AM, OB, EXT, ENT};

/* error types */
enum error_types{
	NO_ERROR, OPEN_FILE, RESERVED_WORD_LABEL_NAME, ILLEGAL_DATA_PARAMETER,
	ILLEGAL_COMMA, MISSING_COMMA, NO_NUMBER_AFTER_SIGN, INVALID_STRING, 
	EXTRA_TEXT_AFTER_STRING, MISSING_FIELDS, INVALID_OPERAND, LABEL_COLON,
	MISSING_OPERAND, EXTRA_TEXT_AFTER_OPERAND, MISSING_LABEL, INVALID_LABEL,
	INVALID_SYNTAX, INVALID_ADDRESS_METHOD, INVALID_NUM_OPERANDS
};

enum addressing_methods{
	INSTANT_ADDRESSING, DIRECT_ADDRESSING, 
	STRUCT_ADDRESSING, REGISTER_ADDRESSING
};

/* Directives */
enum directive_types{NOT_DIRECTIVE, DATA, STRING, STRUCT, ENTRY, EXTERN};

/* Commands */
enum command_types{NOT_CMD, MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC,
                   JMP, BNE, GET, PRN, JSR, RTS, HLT};


/* Flags */
#define OFF 0
#define ON 1
/* Boolean Values */
#define TRUE 1
#define FALSE 0

/* unique values */
#define NONE -1
#define EXIT_FAILURE 1


#endif

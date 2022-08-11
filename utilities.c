
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "assembler.h"

/* remove them to header files later */
#define MAX_APPEND 5
extern int error_type;
extern int error_occurred;
extern int line_number;

extern const char *directives[NUM_DIRECTIVES];
extern const char *commands[NUM_COMMANDS];
extern const char *registers[NUM_REGISTERS];
extern const char base_32[32];

void throw_error(){
	if(error_type == OPEN_FILE){
		fprintf(stderr, "Cannot open the file.");
		return;
	}
	
	switch(error_type){
		case RESERVED_WORD_LABEL_NAME:
			printf("\nIllegal label name in line %d.\n",line_number);
			break;
		case ILLEGAL_DATA_PARAMETER:
			printf("\nIllegal data directive parameters in line %d.\n",line_number);
			break;
		case ILLEGAL_COMMA:
			printf("\nIllegal comma in line %d.\n",line_number);
			break;
		case MISSING_COMMA:
			printf("\nMissing comma in line %d.\n",line_number);
			break;
		case NO_NUMBER_AFTER_SIGN:
			printf("\nNo number after sign in line %d.\n",line_number);
			break;
		case INVALID_STRING:
			printf("\nInvalid string parameter in line %d.\n", line_number);
			break;
		case EXTRA_TEXT_AFTER_STRING:
			printf("\nExtraneous text after string parameter in line %d.\n", line_number);
			break;
		case MISSING_FIELDS:
			printf("\nMissing struct fields in line %d.\n",line_number);
			break;
		case INVALID_OPERAND:
			printf("\nInvalid operand in line %d.\n", line_number);
			break;
		case LABEL_COLON:
			printf("\nMissing label colon in line %d.\n",line_number);
			break;
		case MISSING_OPERAND:
			printf("\nMissing operand in line %d.\n",line_number);
			break;
		case EXTRA_TEXT_AFTER_OPERAND:
			printf("\nExtra text after second operand in line %d.\n",line_number);
			break;
		case MISSING_LABEL:
			printf("\nMissing label name in line %d.\n",line_number);
			break;
		case INVALID_LABEL:
			printf("\nInvalid label name in line %d.\n",line_number);
			break;
		case INVALID_SYNTAX:
			printf("\nInvalid syntax in line %d.\n",line_number);
			break;
		case INVALID_ADDRESS_METHOD:
			printf("\nInvalid addressing method for command in line %d.\n",line_number);
			break;
		case INVALID_NUM_OPERANDS:
			printf("\nInvalid number of operands for command in line %d.\n", line_number);
			break;
		case LABEL_DOESNT_EXIST:
			printf("\nUndefined label in Line: %d.\n",line_number);
			break;
		case EXT_LABEL_EXISTS:
			printf("\nCan't define same extern label twice, line %d.\n",line_number);
			break;
	
	}
	error_occurred = TRUE;
	error_type = NO_ERROR;
	return;
}

char* convert_to_base32(unsigned int word){
	/* each 5 digits in binary is one digit in base 32 */
	char* digits = (char*) malloc(sizeof(char)*BASE_32_DIGITS);
	unsigned int first_digit;
	unsigned int second_digit;
	unsigned int mask = ~((~0) << 5);
	second_digit = word & mask;
	first_digit = ((mask<<5) & word)>>5;
	
	digits[0] = base_32[first_digit];
	digits[1] = base_32[second_digit];
	digits[2] = '\0';

	return digits;
}


char* append_filename(char* filename, int append_type){
	char* appended_filename = (char*)malloc((strlen(filename))+ MAX_APPEND);
	if(!appended_filename){
		fprintf(stderr, "Memory Allocation failed");
		exit(1);
	}
	strcpy(appended_filename, filename);
	switch(append_type){
	
		case AS:
			strcat(appended_filename, ".as");
			break;
		case AM:
			strcat(appended_filename, ".am");
			break;
		case OB:
			strcat(appended_filename, ".ob");
			break;
		case ENT:
			strcat(appended_filename, ".ent");
			break;
		case EXT:
			strcat(appended_filename, ".ext");
			break;					
	}
	return appended_filename;
	
}
/* Gets line as an input and a pointer to the starting character that
   we need to read from.
   returns the next word in the line starting from start_char.  */
char* get_word(char* line, char* word){
	int i = 0;
	
	/* skip whitespaces */
	line = skip_whitespaces(line);
	
	
	if(*line == ','){
		strcpy(word, ",");
		line++; /* point to next char after ',' */
		return line;
	}
	
	/* copy the word */
	
	while(*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0' &&
		  *line != ','){
		word[i++] = *line;
		line++;
	}
	word[i] = '\0';
	return line;
}

void empty_word(char* word){
	
}

int is_reserved_word(char* word){
	int i;
	for(i = 0; i < NUM_DIRECTIVES; i++){
		if(!strcmp(directives[i], word)){
			return TRUE;
		}
	}
	if(is_register(word)){
		return TRUE;
	}
	for(i = 0; i < NUM_COMMANDS; i++){
		if(!strcmp(commands[i], word)){
			return TRUE;
		}
	}
	return FALSE;
}

char* skip_whitespaces(char* line){
	while( *line == ' ' || *line == '\t'){
		line++;
	}
	return line;
}

char* skip_line(char* line){
	while(*line != '\n' || *line != EOF){
		line++;
	}
	return line;
}

int is_end_of_line(char c){
	return (c == '\n' || c == EOF);
}

int is_register(char* operand){
	int i;
	for(i = 0; i < NUM_REGISTERS; i++){
		if(!strcmp(registers[i],operand)){
			return TRUE;
		}
	}
	return FALSE;
}













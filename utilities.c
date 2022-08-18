/************************************
	utilities file
************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utilities.h"
#include "assembler.h"

/*
	This file contains extra functions that are used from a couple other files
	which serve a specific purpose like getting a word from a line, converting
	bases, appending file names, ...
*/

extern int error_type;
extern int error_occurred;
extern int line_number;

extern const char *directives[NUM_DIRECTIVES];
extern const char *commands[NUM_COMMANDS];
extern const char *registers[NUM_REGISTERS];
extern const char base_32[32];

/* 
	This function outputs an error according to the error_type global variable
    then resets it to catch more errors throughout the program.
*/
void throw_error(){
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
			printf("\nInvalid string operand in line %d.\n", line_number);
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
			printf("\nExtra text after operand in line %d.\n",line_number);
			break;
		case MISSING_LABEL:
			printf("\nMissing label name in line %d.\n",line_number);
			break;
		case INVALID_LABEL:
			printf("\nInvalid label name in line %d.\n",line_number);
			break;
		case INVALID_COMMAND_DIRECTIVE:
			printf("\nInvalid command/directive/label in line %d.\n",line_number);
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
		case OPEN_FILE:
			printf("\nCannot open the file.\n");
			break;
		case LONG_LINE:
			printf("\nLine %d has exceeded the maximum supported line length.\n",line_number);
			break;
			
	
	}
	error_occurred = TRUE;
	error_type = NO_ERROR;
	return;
}

/* This function converts a 10 bit word into a base32 two digit word. */
char* convert_to_base32(unsigned int word){
	/* each 5 digits in binary is one digit in base 32 */
	char* digits = (char*) malloc(sizeof(char)*BASE_32_DIGITS);
	unsigned int first_digit;
	unsigned int second_digit;
	/* mask to take the 5 bits from the right. (mask = ...000000011111) */
	unsigned int mask = ~((~0) << 5);
	second_digit = word & mask;
	/* take the left 5 bits so we shift the mask to the left 5 times.
	   then to normalize the value we move them to the right. */
	first_digit = ((mask<<5) & word)>>5;
	
	digits[0] = base_32[first_digit];
	digits[1] = base_32[second_digit];
	digits[2] = '\0';

	return digits;
}

/* This function gets a filename and a defined type and appends the file name
   according to it. (append types: AS,AM,OB,EXT,ENT). */
char* append_filename(char* filename, int append_type){
	char* appended_filename = (char*)malloc((strlen(filename))+ MAX_APPEND);
	if(!appended_filename){
		printf("Memory Allocation failed");
		exit(EXIT_FAILURE);
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
   we need to start reading from.
   returns the next word in the line starting from start_char.  */
char* get_word(char* line, char* word){
	int i = 0;
	/* skip whitespaces */
	line = skip_whitespaces(line);
	
	/* We need to check if commas exist/don't sometimes so we return comma
	   as a next word. */
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

/* Skip whitespaces and return a pointer to the first non whitespace character.*/
char* skip_whitespaces(char* line){
	while( *line == ' ' || *line == '\t'){
		line++;
	}
	return line;
}

/* Check if the given word is reserved word in our assembler. */
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


int is_end_of_line(char c){
	return (c == '\n' || c == '\0');
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

/* Checks if a word is a label with colon at the end. */
int is_label(char* word, char* label_name){
	/* This will hold the label name without the colon. */
	char label_no_colon[MAX_LABEL_LENGTH];
	int length = strlen(word);
	int i = 0;
	/* not valid label length check */
	if(length > MAX_LABEL_LENGTH){
		return FALSE;
	}
	/* get label name without the colon (if there is) . */
	while(i < length && word[i] != ':'){
		label_no_colon[i] = word[i];
		i++;
	}
	label_no_colon[i] = '\0';
	
	/* check if its a valid label name */
	if(!is_label_op(label_no_colon)){
		return FALSE;
	}		
	/* its a label name, but is there a colon? */
	if(word[length - 1] != ':'){
		return FALSE;
	}
	/* copy the label name so we can add it to labels table. */
	while(*word != '\0'){
		*label_name++ = *word++;
	}
	*label_name = '\0';
	return TRUE;	
}

/* This function checks if the word is a label operand 
   (label without a colon). */
int is_label_op(char* word){
	int length = strlen(word);
	int i;
	/* reserved word check. */
	if(is_reserved_word(word)){
		return FALSE;
	}
	/* label must start with alphabetical character. */
	if(!isalpha(*word)){
		return FALSE;
	}
	/* max label length check. */
	if(length > MAX_LABEL_LENGTH){
		return FALSE;
	}
	/* labels alphanumeric characters check. */
	for(i = 0; i<length; i++){
		if(!(isalnum(word[i]))){
			return FALSE;
		}
	}
	return TRUE;
}

/* This function checks if the given word is a struct operand. */
int is_struct_op(char* word){
	/* This will hold label before the dot (ex: S1.2 -> label = "S1"). */
	char* label = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	int length = strlen(word);
	int i = 0;
	if(!label){
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	/* take the label name */
	while(i < length && word[i] != '.'){
		label[i] = word[i];
		i++;
	}
	label[i] = '\0';

	/* if theres no dot then its not a struct */
	if(word[i] != '.'){
		free(label);
		return FALSE;
	}
	/* if its not a label */
	if(!is_label_op(label)){
		free(label);
		return FALSE;
	}
	/* skip the dot */
	i++;
	/* struct has two fields only */
	if(word[i] != '1' && word[i] != '2'){
		free(label);
		return FALSE;
	}
	free(label);
	return TRUE;
}















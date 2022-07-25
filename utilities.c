
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "assembler.h"

/* remove them to header files later */
#define MAX_APPEND 5
extern int error_type;
extern int line_number;

extern const char *directives[NUM_DIRECTIVES];
extern const char *commands[NUM_COMMANDS];
extern const char *registers[NUM_REGISTERS];

void throw_error(){
	if(error_type == OPEN_FILE){
		fprintf(stderr, "Cannot open the file.");
		return;
	}
	
	switch(error_type){
		case RESERVED_WORD_LABEL_NAME:
			printf("Illegal label name in line %d.",line_number);
			break;
		case ILLEGAL_DATA_PARAMETER:
			printf("\nIllegal data directive parameters in line %d.\n",line_number);
			break;
		case ILLEGAL_COMMA:
			printf("\nIllegal comma in line %d.\n",line_number);
	}
	error_type = NO_ERROR;
	return;
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
	while( *line == ' ' || *line == '\t'){
		line++;
	}
	if(*line == ','){
		strcpy(word, ",");
		return ++line;
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
	for(i = 0; i < NUM_REGISTERS; i++){
		if(!strcmp(registers[i],word)){
			return TRUE;
		}
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











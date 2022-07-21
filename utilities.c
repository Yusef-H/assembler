
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "assembler.h"

/* remove them to header files later */
#define MAX_APPEND 5
extern int error_type;

void throw_error(int line_number){
	if(error_type == OPEN_FILE){
		fprintf(stderr, "Cannot open the file.");
		return;
	}
	
	switch(error_type){
	
	}
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
	
	while(*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0'){
		word[i++] = *line;
		line++;
	}
	word[i] = '\0';
	return line;
}

void empty_word(char* word){
	
}




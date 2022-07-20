
#include <stdio.h>
#include <string.h>
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
/* Gets the next word in the line (ignores whitespaces). */
char* get_word(char* line){
	int i;
	int left = 0;
	int right;
	char* word;
	/* skip whitespaces */
	while(line[left] == '0' || line[left] == '\t'){
		left++;
	}
	/*   a bcde e */
	/* Now line[left] holds the first non whitespace character */
	/* Calculating the word's length so we can malloc accordingly. */
	right = left;
	while(line[right] != '0' && line[right] != '\t' && line[right] != '\n'){
		right++;
	}
	word = (char*)malloc((right-left+1)sizeof(char));
	
	/* copy the word */
	for(i = left; i < right; i++){
		word[i] = line[i];
	}
	return word;
}




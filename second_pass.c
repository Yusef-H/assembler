#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utilities.h"
#include "second_pass.h"
#include "first_pass.h"

extern int error_type;
extern int line_number;

void second_pass(FILE* fp_am, label_ptr labels_table){
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	line_number = 1;
	while(fgets(line, MAX_LENGTH, fp_am)){
		/* ignore if empty line or comment */
		if(*line == '\n' || *line == ';')
			continue;
		
		second_parse_line(line, labels_table);
		if(error_type != NO_ERROR){
			throw_error();
			continue;
		}
		line_number++;

	}
	print_labels(labels_table);
	free(line);
}

void second_parse_line(char* line, label_ptr label_table){
	int directive, command;
	char* next_word_start;
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH);
	 
	if(!word){
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	next_word_start = get_word(line, word);
	
	/* skip it if its a label */
	if(is_label(word, word)){
		next_word_start = get_word(next_word_start, word);
	}
	else if((directive = is_directive(word))){
		if(directive != ENTRY){
			return;
		}
		else{
			next_word_start = get_word(next_word_start, word);
			/* if label doesnt exist in labels table already then error */
			if(!turn_label_ent_flag(label_table, word)){
				error_type = LABEL_DOESNT_EXIST;
				return;
			}
		}
	}
	else if((command = is_command(word))){
		
	}
	
}


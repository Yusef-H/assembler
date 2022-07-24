/***********************
	First Pass File
***********************/
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "utilities.h"
#include "first_pass.h"

extern int error_type;
extern word data_segment[SIZE];
extern word code_segment[SIZE];
int IC = 0;
int DC = 0;
int line_number;
/*
	This file handles the first pass on the file
	
	During the first pass:
	- Add all labels to the symbol table.
	- encode the data instructions (.data 1,2,3) to the data segment)
	  using methods from utils file.
	- Check for possible errors and output them.
*/


void first_pass(FILE* fp_am){
	/* Initializing DC and IC. */
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	line_number = 1;
	label_ptr label_table = NULL;
	
	
	while(fgets(line, MAX_LENGTH, fp_am)){
		/* ignore if empty line */
		parse_line(line, &label_table);
		if(error_type != NO_ERROR){
			throw_error();
		}
		 
		line_number++;	
	
	}
	
	free(line);
	print_labels(label_table);
	/* 
	 Update the labels table addresses by adding ic to each one.
	 */
}


void parse_line(char* line, label_ptr* label_table){
	/* Reading and parsing */
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* next_word_start = line;
	
	int label_flag = OFF;
	
	int directive,cmd,label;
	
	/* Initialize empty label table */
	label_ptr label_item;
	char* label_name = (char*)malloc(sizeof(char)*MAX_LENGTH);
	
	
	next_word_start = get_word(line, word);
	if(is_label(word, label_name)){
		if(error_type != NO_ERROR){
			return;
		}
	
/*		if(label_flag == ON){*/
/*			ERROR!*/
/*		}*/

		label_flag = ON;
		label_item = add_label(label_table, label_name);
		
		/* Getting next word in line */
		free(word);
		word = (char*)malloc(sizeof(char)*MAX_LENGTH);
		next_word_start = get_word(next_word_start, word);
	}
	
	/* Next word must be a start of instruction line (command) or
	                     a start of directive line.  */
	
	if((directive = is_directive(word))){
		if(label_flag == ON){
			if(directive == ENTRY || directive == EXTERN){
/*				 delete label item from label table   */
			}
			else{
				label_item->address = DC++;
			}
		/*deal with all directives depending on directive type (switch) dc */
		
		}
		
	}
/*	else if((cmd = is_command(word))){*/
/*		if(label_flag == ON){*/
			/*
			label_item = add_label();
			label_item->is_instruction = TRUE;
			label_item->address = IC;
			 */
/*		}*/
		/* deal with command name (handler).
		   find L while parsing instructions and set IC = L + IC */
/*	}*/
/*	*/
/*	else{*/
		/* error show */
/*	}*/
	
}

int is_label(char* word, char* label_name){
	/* IF TOO LONG ILLEGAL LABEL ERROR*/
	
	char* temp = word;
	char* temp2 = label_name;
	
	/* label must start with an alphabetical character */
	if(!isalpha(*word)){
		return FALSE;
	}
	
	while(isalnum(*temp)){
		temp++;
	}
	if(*temp == ':'){
		temp = word;
		while((*temp) != ':'){
			*temp2 = *temp;
			temp2++;
			temp++;
		}
		*temp2 = '\0';
		/* check for reserved word */
		if(is_reserved_word(label_name)){
			error_type = RESERVED_WORD_LABEL_NAME;
		}
		return TRUE;
	}
	return FALSE;
	
}

int is_directive(char* word){
	if(*word != '.'){
		return NOT_DIRECTIVE;
	}
	if(!strcmp(word,".entry")){
		return ENTRY;
	}
	else if(!strcmp(word,".extern")){
		return EXTERN;
	}
	else if(!strcmp(word,".data")){
		return DATA;
	}
	else if(!strcmp(word,".string")){
		return STRING;
	}
	else if(!strcmp(word,".struct")){
		return STRUCT;
	}
	
	return NOT_DIRECTIVE;

}

int is_command(char* word){
	
}












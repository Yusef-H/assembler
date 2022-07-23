/***********************
	First Pass File
***********************/
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "utilities.h"

extern word data_segment[SIZE];
extern word code_segment[SIZE];
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
	int IC = 0;
	int DC = 0;
	
	int line_number = 1;
	
	
	while(fgets(line, MAX_LENGTH, fp_am)){
		/* ignore if empty line */
		parse_line(line);
		
	
		 
		line_number++;	
	
	}
	free(line);
	
	/* 
	 Update the labels table addresses by adding ic to each one.
	 */
}


void parse_line(char* line){
	/* Reading and parsing */
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* next_word_start = line;
	
	int label_flag = OFF;
	
	int directive,cmd;
	
	next_word_start = get_word(line, word);
	
	if(is_label(word)){
	
/*		if(label_flag == ON){*/
/*			ERROR!*/
/*		}*/

		label_flag = ON;
		/* add label to labels table (STORE IN VARIABLE LABEL_ITEM)*/
	}
	
	if((directive = is_directive(word))){
		if(label_flag == ON){
			if(directive == ENTRY || directive == EXTERN){
				/* delete label item from label table   */
			}
			else{
				/* set label item address to */
			}
		/*deal with all directives depending on directive type (switch) dc */
		
		
		
		}
		
	}
	else if((cmd = is_command(word))){
		if(label_flag == ON){
			/*
			label_item = add_label();
			label_item->is_instruction = TRUE;
			label_item->address = IC;
			 */
		}
		/* deal with command name (handler).
		   find L while parsing instructions and set IC = L + IC */
	}
	
	else{
		/* error show */
	}
	
}












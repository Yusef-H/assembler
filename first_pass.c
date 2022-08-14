/***********************
	First Pass File
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utilities.h"
#include "first_pass.h"


extern int error_type;
extern unsigned int data_segment[SIZE];
extern unsigned int code_segment[SIZE];
extern int IC;
extern int DC;
int line_number;
/*
	This file handles the first pass on the file
	
	During the first pass:
	- Add all labels to the symbol table.
	- encode the directives lines (ex: .data 1,2,3) to the data segment.
	- encode the first word in instructions lines.
	- Check for possible errors and output them.
	
	
	This file has some general functions for handling directives and instructions 
	lines  and makes use of 2 other more specific files which are:
	
	- first_pass_directives.c: Has all of the different directives 
	  specific methods.
	- first_pass_instructions.c: Has all of the different instruction
	  (commands) specific methods.
*/

/* 
  The first pass function
  Gets file as input (after pre assembling), parses the file's lines, then
  updates DC. And the function looks for all kinds of errors in the assembly
  file and outputs them.
*/
label_ptr first_pass(FILE* fp_am){
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	/* Initializing an empty label table */
	label_ptr label_table = NULL;
	
	/* Initialize counters. */
	line_number = 1;
	IC = 0;
	DC = 0;
	

	while(fgets(line, MAX_LENGTH, fp_am)){
		/* ignore if empty line or comment */
		if(*line == '\n' || *line == ';' || *line == '\0')
			continue;
		/* parse each line alone in parse_line function. */
		parse_line(line, &label_table);
		
		/* if there was any error in the line we output it from here. */
		if(error_type != NO_ERROR){
			throw_error();
		} 
		line_number++;	
	}
	free(line);
	
	/* Update the labels table addresses by adding ic to each one. */
	 update_addresses(label_table, IC);
	 
	 /* return the label table so we use it in second pass. */
	 return label_table;
}

/* 
   This function gets a line from the file and labels table that is used
   to store the symbols.
   The function parses the line and looks for errors and outputs them and
   handles multiple type of lines (instruction,data). 
*/
void parse_line(char* line, label_ptr* label_table){
	/* this variable will hold each word in line in order to analyze it. */
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH);
	/* This pointer will point on the next character after we get a word
	   from the line (so we could continue to get next word). */
	char* next_word_start = line;
	/* label pointer to point at the last label added to label table. */
	label_ptr label_item;
	char* label_name = (char*)malloc(sizeof(char)*MAX_LENGTH);
	int label_flag = OFF; 
	/* will hold what type of directive/command we got. */
	int directive,cmd;
	
	/* Get first word in line which could be: Label/command/directive. */
	next_word_start = get_word(line, word);
	
	/* if the line contains only whitespaces then we return and move
	   to next line. */
	if(strlen(word) == 0)
		return;
		
	/* If it's a label */
	if(is_label(word, label_name)){
		int name_len = strlen(label_name);
		label_name[name_len - 1] = '\0';
		label_flag = ON;
		/* add the label to labels table. and keep holding the new added
		   label so we can update the label address/flags... while 
		   analyzing the line. */
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
			/* label that appears before extern or entern directive
			   has no purpose so we delete it. */
			if(directive == ENTRY || directive == EXTERN){
				delete_label(label_table, &label_item);
			}
			else{
				set_label_address(label_item, DC);
			}
		}
		/* Handle the directive according to which directive it is. */
		directive_handler(directive, next_word_start, label_table);

	}
	
	else if((cmd = is_command(word)) != NOT_CMD){
		if(label_flag == ON){
			/* flag that the label is in code segment */
			turn_label_code_flag(label_item);
			set_label_address(label_item, IC);
		}
		/* handle the command according to which command type it is. */
		command_handler(cmd, next_word_start);
	}
	
	else{
		error_type = INVALID_SYNTAX;
		return;
	}	
}

/* This function calls the function that handles the directive type we have. */
void directive_handler(int directive, char* params, label_ptr* label_table){
	switch(directive){
		case DATA:
			data_directive_handler(params);
			break;
		case STRING:
			string_directive_handler(params);
			break;
		case STRUCT:
			struct_directive_handler(params);
			break;
		case ENTRY:
			entry_directive_handler(params);
			break;
		case EXTERN:
			/* Send the labels table so we can add the extern label to it. */
			extern_directive_handler(params, label_table);
			break;
	}
	return;
}

/* This function handles command according to its type. */
void command_handler(int command, char* params){
	/* check MAX_LENGTH (maybe change to something smaller). */
	char* first_operand = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* second_operand = (char*)malloc(sizeof(char)*MAX_LENGTH);
	int L = 0;
	
	/* flags for first and second operands that turn on when we have them
	   in line after the command. */
	int got_first_op=OFF,got_second_op=OFF; 
	
	/* variables that will hold the operands addressing methods types. */
	int first_address_method = NONE, second_address_method = NONE;
	
	/* pointer to the next word start in parameters string */
	char* next_word_start;
	
	if(!first_operand || !second_operand){
		printf("memory allocation failed");
		exit(EXIT_FAILURE);
	}
	
	/* Get first operand */
	next_word_start = get_word(params, first_operand);

	
	/* If first operand exists: */
	if(strlen(first_operand) > 0){
		got_first_op = ON;
		/* get address method of first operand */
		first_address_method = address_method_detector(first_operand);
		/* Update IC */
		if(first_address_method != NONE){
			L = L + method_extra_words(first_address_method);
		}
		
		/* get next word into second_operand which should be , if it exists */
		next_word_start = get_word(next_word_start, second_operand);

		/* if its not empty */
		if(*second_operand != '\n' && *second_operand != '\0'){	
			/* if its not a comma then theres an error.*/
			if(*second_operand != ','){
				error_type = MISSING_COMMA;
				return;
			}
			/* else if its a comma then we get next word which is the 
			   second operand. */
			else{
				next_word_start = get_word(next_word_start, second_operand);
				/* empty second operand check after comma. */
				if(strlen(second_operand) == 0){
					error_type = MISSING_OPERAND;
					return;
				}
				got_second_op = ON;
				second_address_method = address_method_detector(second_operand);
				if(second_address_method != NONE){
					if(first_address_method == REGISTER_ADDRESSING &&
					   second_address_method == REGISTER_ADDRESSING){
					   		/* nothing IC doesnt change here (2 registers) */
					}
					else{
						L = L + method_extra_words(second_address_method);
					}
				}
				/* check for extraneous text error after second operand. */
				next_word_start = get_word(next_word_start, second_operand);
				if(strlen(second_operand) > 0){
					error_type = EXTRA_TEXT_AFTER_OPERAND;
					return;
				}		
			}
		}
	}
		
		
			
	/* validate number of parameters and addressing method types for 
	   the command type. */
	if(validate_num_operands(command, got_first_op, got_second_op)){ 
		if(validate_addressing_methods(command, first_address_method, second_address_method)){	
										 
			/* Create the first instruction word that goes into the 
			   data segment. */
			encode_in_code_segment(create_first_word(command, got_first_op, got_second_op, first_address_method, second_address_method));
			IC = IC + L;
		}
		else{
			error_type = INVALID_ADDRESS_METHOD;
			return;
		}   
	}
	else{
		error_type = INVALID_NUM_OPERANDS;
		return;
	}	
	
}

/* Checks if a string is a command and returns an int representing it. */
int is_command(char* word){
	if(!strcmp(word,"mov"))
		return MOV;
	else if(!strcmp(word,"cmp"))
		return CMP;
	else if(!strcmp(word,"add"))
		return ADD;
	else if(!strcmp(word,"sub"))
		return SUB;
	else if(!strcmp(word,"not"))
		return NOT;
	else if(!strcmp(word,"clr"))
		return CLR;	
	else if(!strcmp(word,"lea"))
		return LEA;	
	else if(!strcmp(word,"inc"))
		return INC;	
	else if(!strcmp(word,"dec"))
		return DEC;	
	else if(!strcmp(word,"jmp"))
		return JMP;	
	else if(!strcmp(word,"bne"))
		return BNE;	
	else if(!strcmp(word,"get"))
		return GET;	
	else if(!strcmp(word,"prn"))
		return PRN;	
	else if(!strcmp(word,"jsr"))
		return JSR;	
	else if(!strcmp(word,"rts"))
		return RTS;	
	else if(!strcmp(word,"hlt"))
		return HLT;	
	else
		return NOT_CMD;
}

/* checks if a string is a valid directive and returns an int representing
   the directive so we can handle it and its parameters. */
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


/* Encodes a value in the data segment. */
void encode_in_data_segment(int value){
/*	printf("\n%d\n",value);*/
	data_segment[DC++] = (unsigned int)value;
}

unsigned int create_first_word(int command,int got_first_op, int got_second_op, int first_address_method, int second_address_method){
	unsigned int first_word = 0;
	/* first we insert the command */
	first_word = command;
	
	
	/* Check if we have one operand */
	if(got_first_op){
		/* create place for operand method bits. */
		first_word = first_word << OPERAND_BITS;
		
		/* If we have second operand */
		if(got_second_op){
			/* add first operand addressing method: */
			first_word = first_word | first_address_method;
			
			/* create new place for the second operand address method */
			first_word = first_word << OPERAND_BITS;
			/* add second method */
			first_word = first_word | second_address_method;
			
		}
		/* else the one operand is destination operand
		   so the 4th and 5th bits are zero */
		else{
			/* Skip the bits that were for source operand */
			first_word = first_word << OPERAND_BITS;
			/* add the addressing method */
			first_word = first_word | first_address_method;
		}
	}
	/* else its a command with no operands */
	else{
		/* skip both operands bits */
		first_word = first_word << 2*OPERAND_BITS;
	
	}
	
	/* add ARE bits which are absolute always in first words */
	first_word = first_word << ARE_BITS;
	
	/* ADD A R E method */
	return first_word;
							   
}

void encode_in_code_segment(unsigned int word){
	code_segment[IC++] = word;
}

/* Checks if a word is a label with colon at the end. */
int is_label(char* word, char* label_name){

	/* This will hold the label name without the colon. */
	char* label_no_colon = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH+1);
	int length = strlen(word);
	int i = 0;
	if(!label_no_colon){
		printf("memory allocation failed");
		exit(1);
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











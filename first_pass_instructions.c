/***********************************
	First Pass Instructions File
************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utilities.h"
#include "first_pass.h"
#include "assembler.h"

/*
	This file has methods that are used in parsing/validating instruction lines 
*/

extern int error_type;
extern unsigned int code_segment[SIZE];
extern int IC;


/* This method gets an operand and returns the number of the addressing
   method. */
int address_method_detector(char* op){	
	
	/* instant addressing - example: #7, #-5... */
	if(*op == '#'){
		op++;
		/* number check */
		if(*op == '+' || *op == '-'){
			op++;
			if(!(isdigit(*op))){
				return NONE;
			}
		}
		/* # appeard without numbers */
		if(!(isdigit(*op))){
				op++;
				return NONE;
		}
		while(*op != '\n' && *op != EOF && *op != '\0'){
			if(!(isdigit(*op))){
				op++;
				return NONE;
			}
			op++;
		}
		return INSTANT_ADDRESSING;
	}
	
	/* direct addressing */	
	else if(is_label_op(op)){
		return DIRECT_ADDRESSING;
	}
	
	/* struct addressing */
	else if(is_struct_op(op)){
		return STRUCT_ADDRESSING;	
	}

	/* register addressing */
	else if(is_register(op)){
		return REGISTER_ADDRESSING;
	}
	return NONE;
}

/* This function returns the number of extra words the addressing method
   requires. */
int method_extra_words(int method){
	if(method == STRUCT_ADDRESSING)
		return 2;
	/* direct or instant or register addressing */	
	return 1;
}

unsigned int create_first_word(int command,int got_first_op, int got_second_op, int first_address_method, int second_address_method){
	unsigned int first_word = EMPTY_WORD;
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
	/* add ARE bits which are absolute always in first words. */
	first_word = first_word << ARE_BITS;
	return first_word;							   
}

/* encode the given word in code segment array. */
void encode_in_code_segment(unsigned int word){
	code_segment[IC++] = word;
}

/* This function checks if the number of operands received are valid for 
   the command. */
int validate_num_operands(int command,int got_first_op,int got_second_op){
	switch(command){
		/* commands that accept 2 operands: */
		case MOV:
		case CMP:
		case ADD:
		case SUB:
		case LEA:
			return got_first_op && got_second_op;
			
		/* commands that accept 1 operand */	
		case NOT:
		case CLR:
		case INC:
		case DEC:
		case JMP:
		case BNE:
		case GET:
		case PRN:
		case JSR:
			return got_first_op && !got_second_op;
		
		/* commands that accept no operands: */
		case RTS:
		case HLT:
			return !got_first_op && !got_second_op;
	}

	return FALSE;
}

/* This function checks if the addressing methods are valid for the command. */
int validate_addressing_methods(int command,int first_address_method,
								int second_address_method){
	switch(command){
		/* all methods supported (number of operands validation done 
		   in another function)
		   or commands with no operands so we return true because the only
		   check we need to do is check number of operands for them. */
		case CMP:
		case PRN:
		case RTS:
		case HLT:
			return TRUE;
		
		/* commands that accept:
		   Src: 0,1,2,3
		   Dest: 1,2,3 */
		case MOV:
		case ADD:
		case SUB:
		return (first_address_method == INSTANT_ADDRESSING ||
		       first_address_method == DIRECT_ADDRESSING ||
		       first_address_method == STRUCT_ADDRESSING ||
		       first_address_method == REGISTER_ADDRESSING) &&
		       (second_address_method == DIRECT_ADDRESSING ||
		       second_address_method == STRUCT_ADDRESSING ||
		       second_address_method == REGISTER_ADDRESSING);
		       
		/* commands that accept: 
		   Src: doesn't have source operand.
		   Dest: 1,2,3*/
		case NOT:
		case CLR:
		case INC:
		case DEC:
		case JMP:
		case BNE:
		case GET:
		case JSR:
		return (first_address_method == DIRECT_ADDRESSING ||
		       first_address_method == STRUCT_ADDRESSING ||
		       first_address_method == REGISTER_ADDRESSING);
		
		/* lea command:
		   Src: 1,2
		   Dest: 1,2,3 */
		case LEA:
		return (first_address_method == DIRECT_ADDRESSING ||
		       first_address_method == STRUCT_ADDRESSING) &&
		       (second_address_method == DIRECT_ADDRESSING ||
		       second_address_method == STRUCT_ADDRESSING ||
		       second_address_method == REGISTER_ADDRESSING);
	}
	return FALSE;
}




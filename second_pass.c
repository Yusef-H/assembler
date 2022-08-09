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
extern int IC;
extern int code_segment[SIZE];

void second_pass(FILE* fp_am, label_ptr labels_table){
	int i;
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	line_number = 1;
	IC = 0;
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
	
	printf("\n\n");
	for(i = 0; i<100; i++){
		printf("%d ",code_segment[i]);	
	}
	free(line);
}

void second_parse_line(char* line, label_ptr label_table){
	int directive, command;
	char* next_word_start;
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char label[30];
	 
	if(!word){
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	next_word_start = get_word(line, word);
	/* skip it if its a label */
	if(is_label(word, label)){
		next_word_start = get_word(next_word_start, word);
	}
	if(strlen(word) == 0)
		return;
	if((directive = is_directive(word))){
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
	else if((command = is_command(word)) != NOT_CMD){
		second_pass_command_handler(command, next_word_start);
	}
	
}

void second_pass_command_handler(int command, char* params){
	char* next_word_start = params;
	char first_operand[MAX_LENGTH];
	char second_operand[MAX_LENGTH];
	int num_operands = 0; 
	int first_address_method = NONE, second_address_method = NONE;
	
	num_operands = command_num_operands(command);
	
	next_word_start = get_word(next_word_start, first_operand);
/*	printf("\nOle: %s\n\n ",params);*/
	if(num_operands == 2){
		/* skip comma */
		next_word_start = get_word(next_word_start, second_operand);
		next_word_start = get_word(next_word_start, second_operand);
		first_address_method = address_method_detector(first_operand);
		second_address_method = address_method_detector(second_operand);
	}
	else if(num_operands == 1){
		first_address_method = address_method_detector(first_operand);
	}
	/* first word encoded in first pass. */
	IC++;
	encode_extra_words(num_operands, first_address_method, second_address_method, first_operand, second_operand);

}

void encode_extra_words(int num_operands, int first_address_method, int second_address_method, char* first_op, char* second_op){
	if(num_operands == 1){
		encode_op_word(first_op, first_address_method, FALSE);
	}
	else if(num_operands == 2){
		encode_op_word(first_op, first_address_method, TRUE);
		encode_op_word(second_op, second_address_method, FALSE);
	}
}

void encode_op_word(char* operand, int method, int is_src_op){
	unsigned int word = 0;
	switch(method){
		case INSTANT_ADDRESSING:
			word = (unsigned)atoi((operand + 1));
			encode_in_code_segment(word);
			break;
		case DIRECT_ADDRESSING:	
			IC++;
		
			break;
		case STRUCT_ADDRESSING:
			IC = IC + 2;
			break;
		case REGISTER_ADDRESSING:
			IC++;
			break;
	}
}

int command_num_operands(int command){
	switch(command){
		/* commands that accept 2 operands: */
		case MOV:
		case CMP:
		case ADD:
		case SUB:
		case LEA:
			return 2;
			
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
			return 1;
		
		/* commands that accept no operands: */
		case RTS:
		case HLT:
			return 0;
	}
	/* ERRORRRRRRRRRRRRRRRRRRRRRRR */
	return NONE;
}






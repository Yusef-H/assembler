/********************************
	First Pass Directives File
*********************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utilities.h"
#include "first_pass.h"

extern int error_type;
extern unsigned int data_segment[SIZE];
extern int DC;

/* 
   Entry directive handler for first pass.
   In this function we check for syntax errors in the directive line.
   (The check for defined labels or not is done in the second pass after
   storing all the defined labels in the labels table). 
*/
void entry_directive_handler(char* params){
	char label_name[MAX_LABEL_LENGTH];
	/* next param after the directive should be the label name. */
	params = get_word(params, label_name);
	if(strlen(label_name) == 0){
		error_type = MISSING_LABEL;
		return;
	}
	if(!is_label_op(label_name)){
		error_type = INVALID_LABEL;
		return;
	}
	/* check for extra text. (according to maman instructions there should
	   						  be only 1 label). */
	params = get_word(params, label_name);
	if(strlen(label_name) > 0){
		error_type = EXTRA_TEXT_AFTER_OPERAND;
		return;
	}
}

/* 
   Extern directive handler.
   Here we get the labels table so we can add the label parameter
   into it as an extern label with zero address. 
*/
void extern_directive_handler(char* params, label_ptr* label_table){
	label_ptr extern_label_ptr;
	char label_name[MAX_LABEL_LENGTH];
	/* Syntax check is the same as entry directive */
	entry_directive_handler(params);
	/* if there was an error return */
	if(error_type != NO_ERROR)
		return; 
	params = get_word(params, label_name);
	extern_label_ptr = add_label(label_table, label_name);
	turn_label_ext_flag(extern_label_ptr);
	set_label_address(extern_label_ptr, 0);
}

/*
	String directive handler.
	This function checks if the string directive parameter is a valid string,
	starts with " and ends with " with whatever inbetween.
*/
void string_directive_handler(char* string){
	string = skip_whitespaces(string);
	if(*string != '"'){
		error_type = INVALID_STRING;
		return;
	}
	string++;
	while( !(is_end_of_line(*string)) && *string != '"'){
		encode_in_data_segment((unsigned int)*string);
		string++;
	}
	if(*string != '"'){
		error_type = INVALID_STRING;
		return;
	}
	string++;
	string = skip_whitespaces(string);
	if(!(is_end_of_line(*string))){
		error_type = EXTRA_TEXT_AFTER_STRING;
		return;
	}
	encode_in_data_segment('\0'); /* end of legal string */
}

/*
 	Struct directive handler.
	This function validates that struct directive have number then 
	string parameter seperated by a comma.  	
*/
void struct_directive_handler(char* params){
	char number_param[MAX_LENGTH];
	char* number_ptr = number_param; /* Used to copy from params to number parameter. */
	params = skip_whitespaces(params);
	
	/* No parameters check. */
	if(is_end_of_line(*params)){
		error_type = MISSING_FIELDS;
		return;
	}
	/* Handling number with + or -. */
	if(*params == '+' || *params == '-'){
		*number_ptr++ = *params++;
	}
	/* get number parameter */
	while(isdigit(*params)){
		*number_ptr++ = *params++;
	}
	/* must find comma after number parameter. */
	params = skip_whitespaces(params);
	if(*params != ','){
		error_type = MISSING_COMMA;
		return;
	}
	params++;
	/* check for illegal comma, missing string parameter */
	params = skip_whitespaces(params);
	if(*params == ','){
		error_type = ILLEGAL_COMMA;
		return;
	}
	if(is_end_of_line(*params)){
		error_type = MISSING_FIELDS;
		return;
	}
	
	encode_in_data_segment(atoi(number_param));
	
	/* we use the string handler for next parameter since it must be a 
	   string and needs same checks/encoding. */
	string_directive_handler(params);
}

/*
	Data Directive Handler function.
	Encodes the data directives in the data segment while checking
	for errors.
*/
void data_directive_handler(char* params){
	char* temp_params = params; /* Will point to the next parameter */
	char* number; /* holds number parameter to encode them using atoi*/
	char* temp_num; /* pointer to the number parameter */
	char next_char; /* holds next char after reading a number parameter */
	int got_num_flag; /* flag that gets turned on when we get a number */
	
	while(*temp_params != '\n' && *temp_params != '\0' && *temp_params != EOF){
		got_num_flag = OFF; /* initialize flag */
		
		/* new number parameter */
		number = (char*)malloc(sizeof(char)*MAX_LENGTH); 
		if(!number){
			printf("Memory allocation failed.");
			exit(EXIT_FAILURE);
		}
		
		/* skip whitespaces */
		temp_params = skip_whitespaces(temp_params);
		
		/* point temp_num at number memory start to start copying */
		temp_num = number;

		/* Error checks for data parameters*/
		if(!(isdigit(*temp_params))){ /* if not a number */		
			/* if its none of these chars then its illegal */
			if(*temp_params != '+' && *temp_params != '-' &&
			   *temp_params != '\n' && *temp_params != '\0' &&
			   *temp_params != ','){

			   	temp_params++;
				error_type = ILLEGAL_DATA_PARAMETER;
				return;
			}
			
			/* handle comma and its errors */
			if(*temp_params == ','){
				temp_params++;
				temp_params = skip_whitespaces(temp_params);
				/* after comma its either new line or a parameter */
				if(*temp_params == '\n'){
					error_type = ILLEGAL_COMMA;
					temp_params++;
					return;
				}
				/* illegal extra comma */
				if(*temp_params == ','){
					error_type = ILLEGAL_COMMA;
					temp_params++;
					return;
				}
			}
			
			/* handle + and - */
			if(*temp_params == '+' || *temp_params == '-'){
				*temp_num++ = *temp_params++;
				/* Must be a digit after + or - */
				if(!isdigit(*temp_params)){
					error_type = NO_NUMBER_AFTER_SIGN;
					return;
				}
			}
		}
		
		skip_whitespaces(temp_params);
		while(isdigit(*temp_params)){
			got_num_flag = ON;
			*temp_num++ = *temp_params++;
		}
		*temp_num = '\0';
		
		/* next non whitespace char after a number can be either a comma
		   or a new line character if end of line or eof if end of file */
		next_char = *(skip_whitespaces(temp_params));
		if(isdigit(next_char) && got_num_flag){
			error_type = MISSING_COMMA;
			return;
		}
		if(next_char != ',' && next_char != '\n' && next_char != EOF){
			error_type = ILLEGAL_DATA_PARAMETER;
			return;
		}
		if(got_num_flag)
			encode_in_data_segment(atoi(number));
		free(number);
	}
}


/* Encodes a value in the data segment. */
void encode_in_data_segment(int value){
	data_segment[DC++] = (unsigned int)value;
}



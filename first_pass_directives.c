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


void entry_directive_handler(char* params){
	/* Here we check if the syntax has no errors, in the second pass
	   we check if the label exists (been defined in the file). */
	char* label_name = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	if(!label_name){
		printf("Memory allocation failed.");
		exit(1);
	}
	params = get_word(params, label_name);
	
	if(strlen(label_name) == 0){
		error_type = MISSING_LABEL;
		return;
	}
	if(!is_label_op(label_name)){
		error_type = INVALID_LABEL;
		return;
	}
	params = get_word(params, label_name);
	if(strlen(label_name) > 0){
		error_type = EXTRA_TEXT_AFTER_OPERAND;
		return;
	}
}


void extern_directive_handler(char* params, label_ptr* label_table){
	label_ptr extern_label_ptr ;
	char* label_name = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
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
			exit(1);
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
				throw_error();
				return;
			}
			
			/* handle comma and its errors */
			if(*temp_params == ','){
				temp_params++;
				temp_params = skip_whitespaces(temp_params);
				/* after comma its either new line or a parameter */
				if(*temp_params == '\n'){
					error_type = ILLEGAL_COMMA;
					throw_error();
					temp_params++;
					return;
				}
				/* illegal extra comma */
				if(*temp_params == ','){
					error_type = ILLEGAL_COMMA;
					throw_error();
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
					throw_error();
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
			throw_error();
			return;
		}
		if(next_char != ',' && next_char != '\n' && next_char != EOF){
			error_type = ILLEGAL_DATA_PARAMETER;
			throw_error();
			return;
		}
		if(got_num_flag)
			encode_in_data_segment(atoi(number));
		free(number);

	}
}

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

void struct_directive_handler(char* params){
	char* number_param = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* number_ptr = number_param;
	if(!number_param){
		printf("Memory allocation failed");
		exit(1);
	}
	
	params = skip_whitespaces(params);
	if(is_end_of_line(*params)){
		error_type = MISSING_FIELDS;
		return;
	}
	if(*params == '+' || *params == '-'){
		*number_ptr++ = *params++;
	}
	/* get number parameter */
	while(isdigit(*params)){
		*number_ptr++ = *params++;
	}
	/* must find comma after */
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
	
	string_directive_handler(params);
}

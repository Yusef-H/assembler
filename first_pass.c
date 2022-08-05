/***********************
	First Pass File
***********************/
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "utilities.h"
#include "first_pass.h"

extern int error_type;
extern unsigned int data_segment[SIZE];
extern unsigned int code_segment[SIZE];
int IC = 0;
int DC = 0;
int line_number;
/*
	This file handles the first pass on the file
	
	During the first pass:
	- Add all labels to the symbol table.
	- encode the data instructions (.data 1,2,3) to the data segment).
	- Check for possible errors and output them.
*/


void first_pass(FILE* fp_am){
	int i;
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	label_ptr label_table = NULL;
	line_number = 1;
	
	
	
	while(fgets(line, MAX_LENGTH, fp_am)){
		/* ignore if empty line */
		if(*line == '\n' || *line == ';')
			continue;
		parse_line(line, &label_table);
		if(error_type != NO_ERROR){
			throw_error();
		}
		 
		line_number++;	
	
	}
	
	free(line);
	print_labels(label_table);
	printf("\n\n");
	for(i=0; i<DC; i++){
		printf("%u ",data_segment[i]);
	}
	printf("\n\n");
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
	
	
	/*** HERE  *************************** ***/
		if(error_type != NO_ERROR){
			throw_error();
			return;
		}

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
			/* label that appears before extern or entern directive
			   has no purpose so we delete it. */
			if(directive == ENTRY || directive == EXTERN){
				delete_label(label_table, &label_item);
			}
			else{
				label_item->address = DC;
			}
			
		/*
		deal with all directives depending on directive type (switch) dc 
		*/	
			
		
		
		}
		directive_handler(directive, next_word_start, label_table);
		
	}
	else if((cmd = is_command(word))){
		if(label_flag == ON){
			
			label_item->code_flag = ON;
			label_item->address = IC;
			
		}
		IC++;
		/* deal with command name (handler).
		   find L while parsing instructions and set IC = L + IC */
		command_handler(cmd, next_word_start);
	}
/*	*/
/*	else{*/
/*		 error show */
/*	}*/
	
}


int is_label(char* word, char* label_name){

	
	char* label_no_colon = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH+1);
	int length = strlen(word);
	int i = 0;
	if(!label_no_colon){
		printf("memory allocation failed");
		exit(1);
	}
	while(i < length && word[i] != ':'){
		label_no_colon[i] = word[i];
		i++;
	}
	label_no_colon[i] = '\0';
	if(!is_label_op(label_no_colon)){
		return FALSE;
	}
		
		
	/* its a label name, but is there a colon? */
	if(word[length - 1] != ':'){

		error_type = LABEL_COLON;
		throw_error();
		return FALSE;
	}
	while(*word != '\0'){
		*label_name++ = *word++;
	}
	*label_name = '\0';
	return TRUE;
	
	
	
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
			/* Syntax check */
			extern_directive_handler(params, label_table);
			break;
	
	}
	return;
}

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
	extern_label_ptr -> ext_flag = ON;
}

/*
	Data Directive Handler function.
	Encodes the data directives in the data segment while carefully checking
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
		temp_num = '\0';
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
	if(*params == '+' || *params == '-')
		params++;
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

void encode_in_data_segment(int value){
	data_segment[DC++] = (unsigned int)value;
}


void command_handler(int command, char* params){
	/* check MAX_LENGTH (maybe change to something smaller). */
	char* first_operand = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* second_operand = (char*)malloc(sizeof(char)*MAX_LENGTH);
	int first_address_method = NONE, second_address_method = NONE;
	char* next_word_start;
	if(!first_operand || !second_operand){
		printf("memory allocation failed");
		exit(1);
	}
	
	next_word_start = get_word(params, first_operand);
	/* first op exists in instruction line */
	if(strlen(first_operand) > 0){
	
		first_address_method = address_method_detector(first_operand);
		if(first_address_method != NONE){
			IC = IC + method_extra_words(first_address_method);
		}

		next_word_start = get_word(next_word_start, second_operand);

		if(*second_operand != '\n' && *second_operand != '\0'){	
			if(*second_operand != ','){
				error_type = MISSING_COMMA;
				throw_error();
				return;
			}
			else{
				next_word_start = get_word(next_word_start, second_operand);
				if(strlen(second_operand) == 0){
					error_type = MISSING_OPERAND;
					throw_error();
					return;
				}
				second_address_method = address_method_detector(second_operand);
				if(second_address_method != NONE){
					if(first_address_method == REGISTER_ADDRESSING &&
					   second_address_method == REGISTER_ADDRESSING){
					   		/* nothing IC doesnt change here (2 registers) */
					   }
					else{
						IC = IC + method_extra_words(second_address_method);
					}
				}
				
				next_word_start = get_word(next_word_start, second_operand);
				if(strlen(second_operand) > 0){
					error_type = EXTRA_TEXT_AFTER_OPERAND;
					throw_error();
					return;
				}

				/* CHECK AFTER SECOND OPERAND FOR ERRORS */			
			}
		}
		else{
			
		}
		
	}
	
	
	/* IF ADDRESS METHODS ARE AVAILABLE FOR THIS COMMAND 
	        IF number of operands suits the command..
	       	 	encode to instruction ds
	        ELSE
	       		error
	   ELSE
	   		error
	 */
	 
	
	
}

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
				error_type = INVALID_OPERAND;
				throw_error();
				return NONE;
			}
		}
		while(*op != '\n' && *op != EOF && *op != '\0'){
			if(!(isdigit(*op))){
				op++;
				error_type = INVALID_OPERAND;
				throw_error();
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

int method_extra_words(int method){
	if(method == STRUCT_ADDRESSING)
		return 2;
	/* direct or instant or register addressing */	
	return 1;
}

int is_label_op(char* operand){
	int length = strlen(operand);
	int i;
	if(is_reserved_word(operand)){
		return FALSE;
	}
	if(!isalpha(*operand)){
		return FALSE;
	}
	if(length > MAX_LABEL_LENGTH){
		return FALSE;
	}
	for(i = 0; i<length; i++){
		if(!(isalnum(operand[i]))){
			return FALSE;
		}
	}
	return TRUE;
}

int is_struct_op(char* operand){
	/* These will hold label before . and number after
	   Example: S.2 -> label = "s", number = "." */
	char* label = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	int length = strlen(operand);
	int i = 0;
	if(!label){
		printf("Memory allocation failed");
		exit(1);
	}
	/* take the label name */
	while(i < length && operand[i] != '.'){
		label[i] = operand[i];
		i++;
	}
	label[i] = '\0';

	/* if theres no dot then its not a struct */
	if(operand[i] != '.')
		return FALSE;
	
	/* if its not a label */
	if(!is_label_op(label))
		return FALSE;
	/* skip the dot */
	i++;
	
	/* struct has two fields only */
	if(operand[i] != '1' && operand[i] != '2')
		return FALSE;
	
	return TRUE;
	
}











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
		parse_line(line, &label_table);
		if(error_type != NO_ERROR){
			throw_error();
		}
		 
		line_number++;	
	
	}
	
	free(line);
	print_labels(label_table);
	
	for(i=0; i<DC; i++){
		printf("%u ",data_segment[i]);
	}
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
				label_item->address = DC;
			}
			
		/*
		deal with all directives depending on directive type (switch) dc 
		*/	
			directive_handler(directive, next_word_start);
		
		
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

void directive_handler(int directive, char* params){
	switch(directive){
		case DATA:
			data_directive_handler(params);
			break;
		case STRING:
			/*string_directive_handler(params);*/
			break;
		case STRUCT:
			/*struct_directive_handler(params);*/
			break;
		/* ext ent */
	
	}
	return;
}
/*
	Data Directive Handler function.
	Encodes the data directives in the data segment while carefully checking
	for errors and printing them.
*/
void data_directive_handler(char* params){
	char* temp_params = params;
	char* number;
	char* temp_num;
	int got_num_flag;
	while(*temp_params != '\n' && *temp_params != '\0' && *temp_params != EOF){
		got_num_flag = OFF;
		
		number = (char*)malloc(sizeof(char)*MAX_LENGTH);
		if(!number){
			printf("Memory allocation failed.");
			exit(1);
		}
		temp_params = skip_whitespaces(temp_params);
		temp_num = number;

		/* Error checks */
		if(!(isdigit(*temp_params))){

			if(*temp_params != '+' && *temp_params != '-' &&
			   *temp_params != '\n' && *temp_params != '\0' &&
			   *temp_params != ','){

			   	temp_params++;
				error_type = ILLEGAL_DATA_PARAMETER;
				throw_error();
				return;
			}
			/* deal with + or - */
			if(*temp_params == '+' || *temp_params == '-'){
				*temp_num++ = *temp_params++;
			}

			/* deal with comma and its errors */
			if(*temp_params == ','){
				temp_params++;
				temp_params = skip_whitespaces(temp_params);
				if(*temp_params == '\n'){
					error_type = ILLEGAL_COMMA;
					throw_error();
					temp_params++;
					return;
				}
				if(*temp_params == ','){
					error_type = ILLEGAL_COMMA;
					throw_error();
					temp_params++;
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
		if(isdigit(*(skip_whitespaces(temp_params))) && got_num_flag){
			error_type = MISSING_COMMA;
			throw_error();
			return;
		}
		encode_in_data_segment(atoi(number));
		free(number);

	}
}

		
	



void encode_in_data_segment(int value){
	data_segment[DC++] = (unsigned int)value;
}











/****************************************
		Second Pass File
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "utilities.h"
#include "second_pass.h"
#include "first_pass.h"

extern int error_type;
extern int error_occurred;
extern int line_number;
extern int IC;
extern int DC;
extern int code_segment[SIZE];
extern int data_segment[SIZE];

/*
	Second Pass function.
	This function runs the second pass on the .am file, parses each line
	and encodes the extra words for instruction lines while checking for errors.
	at the end if there was no errors throughout all of the program, it creates
	the .ob file, and if there was any entry and extern labels we create the
	.ent .ext files. 
*/
void second_pass(FILE* fp_am, label_ptr labels_table, char* file_name){
	/* flags to check if there was any enternal or external labels */
	int ext_exist = OFF,ent_exist = OFF;
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* line_position = line;
	line_number = 1;
	IC = 0;
	while(fgets(line, MAX_LENGTH, fp_am)){
		line_position = skip_whitespaces(line);
		/* ignore if empty line or comment */
		if(*line_position == '\n' || *line_position == ';' || *line_position == '\0')
			continue;
			
		/* parse the line according to second pass algorithm. */
		second_parse_line(line, labels_table);
		
		/* If there was an error while parsing the line, throw it. */
		if(error_type != NO_ERROR){
			throw_error();
			continue;
		}
		line_number++;
	}
	
	/* Update extern labels addresses. (to start at 100) */
	update_ext_addresses(labels_table);
	
	print_labels(labels_table);
	
	/* Check if there exists any extern or entry labels to decide whether
	   to create .ent and .ent files. */
	ext_exist = ext_exist_check(labels_table);
	ent_exist = ent_exist_check(labels_table);
	
	/* create output files if there was no error only. */
	if(!error_occurred){
		output_files_handler(file_name,labels_table,ext_exist,ent_exist);
	}
}

/*
	
*/
void second_parse_line(char* line, label_ptr label_table){
	int directive, command; /* will hold cmd,directive types. */
	char* next_word_start; /* points to next char in line after getting word. */
	char* word = (char*)malloc(sizeof(char)*MAX_LENGTH); /* to hold a word at once and analyze it. */
	char label[MAX_LABEL_LENGTH];
	if(!word){
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	next_word_start = get_word(line, word);
	
	/* skip it if its a label definition */
	if(is_label(word, label)){
		next_word_start = get_word(next_word_start, word);
	}
	
	/* Now the word should be either directive or command. */
	
	if((directive = is_directive(word))){
		/* We only need to mark entry directives and check if the label
		   is defined by checking our labels table we created in first pass. */
		if(directive != ENTRY){
			return;
		}
		else{
			/* get the label. */
			next_word_start = get_word(next_word_start, word);
			/* if label doesnt exist in labels table already then error */
			if(!turn_label_ent_flag(label_table, word)){
				error_type = LABEL_DOESNT_EXIST;
				return;
			}
		}
		
	}
	else if((command = is_command(word)) != NOT_CMD){
		/* Send to command handler in second pass to encode extra words. */
		second_pass_command_handler(command, next_word_start, label_table);
	}
	
}

void output_files_handler(char* file_name,label_ptr labels_table, int ext, int ent){
	create_object_file(file_name);
	if(ext){
		create_ext_file(file_name, labels_table);	
	}
	if(ent){
		create_ent_file(file_name,labels_table);
	}
}

void create_object_file(char* file_name){
	FILE* obj_file;
	int i = 0;
	unsigned int address = MEMORY_START;
	char* temp1 = convert_to_base32(IC);
	char* temp2 = convert_to_base32(DC);
	if(temp1[0] == '!'){
		temp1[0] = ' ';
	}
	if(temp2[0] == '!'){
		temp2[0] = ' ';
	}
	file_name = append_filename(file_name,OB);
	obj_file = fopen(file_name, "w");
	/* encoding IC and DC at start of object file */
	fprintf(obj_file, "%s\t\t%s\n\n",temp1,temp2);
	free(temp1);
	free(temp2);
	/* create code segment */
	while(i < IC){
		temp1 = convert_to_base32(address++);
		temp2 = convert_to_base32(code_segment[i++]);
		fprintf(obj_file, "%s\t\t%s\n",temp1,temp2);
		free(temp1);
		free(temp2);
	}
	
	i = 0;
	
	while(i < DC){
		temp1 = convert_to_base32(address++);
		temp2 = convert_to_base32(data_segment[i++]);
		fprintf(obj_file, "%s\t\t%s\n",temp1,temp2);
		free(temp1);
		free(temp2);
	}	
	fclose(obj_file);
}

void create_ext_file(char* file_name,label_ptr labels_table){
	FILE* ext_file;
	label_ptr temp = labels_table;
	file_name = append_filename(file_name, EXT);
	ext_file = fopen(file_name, "w");
	while(temp){
		if(temp->ext_flag){
			address_ptr addresses = temp->addresses;
			while(addresses){
				char* address = convert_to_base32(addresses->address);
				fprintf(ext_file,"%s\t%s\n",temp->label_name, address);
				free(address);
				addresses = addresses->next;
			}
			
		}
		temp = temp->next;
	}
	fclose(ext_file);
}

void create_ent_file(char* file_name, label_ptr labels_table){
	FILE* ent_file;
	label_ptr temp = labels_table;
	file_name = append_filename(file_name, ENT);
	ent_file = fopen(file_name, "w");
	while(temp){
		if(temp->ent_flag){
			char* address = convert_to_base32(temp->address);
			fprintf(ent_file,"%s\t%s\n",temp->label_name, address);
			free(address);
		}
		temp = temp->next;
	}
	fclose(ent_file);
}

/*
	Command handler second pass.
	This function checks the number of operands and gets them and their 
	addressing methods, then sends them to encoding function.
	
*/
void second_pass_command_handler(int command, char* params, label_ptr label_table){
	char* next_word_start = params;
	char first_operand[MAX_LENGTH];
	char second_operand[MAX_LENGTH];
	int num_operands = 0; 
	int first_address_method = NONE, second_address_method = NONE;
	
	/* Get the needed number of operands for the command type we have. */
	num_operands = command_num_operands(command);
	
	/* Validation has been done in first pass so I don't need to do it again. */
	
	/* Get the first operand. */	
	next_word_start = get_word(next_word_start, first_operand);

	if(num_operands == 2){
		/* Skip the comma */
		next_word_start = get_word(next_word_start, second_operand);
		/* Get the second operand. */
		next_word_start = get_word(next_word_start, second_operand);
		/* Get the operands addresses methods. */
		first_address_method = address_method_detector(first_operand);
		second_address_method = address_method_detector(second_operand);
	}
	else if(num_operands == 1){
		/* Get the address method of the only operand. */
		first_address_method = address_method_detector(first_operand);
	}
	/* First word has been encoded in first pass, so we skip 1 by increasting IC. */
	IC++;
	/* Encode the extra words in code segment. */
	encode_extra_words(num_operands, first_address_method, second_address_method, first_operand, second_operand, label_table);
}

/*
	Encode extra words function.
	This function handles the encoding of the extra words of the instruction
	line based on the given parameters.
*/
void encode_extra_words(int num_operands, int first_address_method, int second_address_method, char* first_op, char* second_op, label_ptr label_table){
	/* If both of the operands are registers, we handle them here so we encode only
	   one word unlike other situations. */
	if(first_address_method == REGISTER_ADDRESSING && second_address_method == REGISTER_ADDRESSING){
		handle_register_encoding(first_op, second_op, TRUE, TRUE);
	}
	else if(num_operands == 1){
		encode_op_word(first_op, first_address_method, FALSE, label_table);
	}
	else if(num_operands == 2){
		encode_op_word(first_op, first_address_method, TRUE, label_table);
		encode_op_word(second_op, second_address_method, FALSE, label_table);
	}
}

/*
	Encode operand word function.
	This function gets an operand and encodes the operand's word in code segment
	depending on its addressing method and if its a source or dest operand, which
	are given as parameters.
*/
void encode_op_word(char* operand, int method, int is_src_op, label_ptr label_table){
	unsigned int word = EMPTY_WORD;
	char* temp;
	switch(method){
		case INSTANT_ADDRESSING:
			/* Skip the # and get the number. */
			word = (unsigned)atoi((operand + 1));
			/* Absolute ARE for instant addressing. */
			word = encode_ARE(word,ABSOLUTE);
			encode_in_code_segment(word);
			break;
			
		case DIRECT_ADDRESSING:
			handle_label_encoding(operand, label_table);	
			break;
			
		case STRUCT_ADDRESSING:
			/* get struct label */
			temp = operand;
			while(*temp != '.'){
				temp++;
			}
			*temp = '\0';
			/* encode the label. */
			handle_label_encoding(operand, label_table);
			/* Bring back the dot and skip it to the struct field number. */
			*temp++ = '.';
			word = (unsigned)atoi(temp);
			word = encode_ARE(word,ABSOLUTE);
			encode_in_code_segment(word);
			break;
			
		case REGISTER_ADDRESSING:
			/* Handle register encoding when there's no 2 registers in instruction
			   line. (the former has been handled alone before). */
			handle_register_encoding(operand, operand, is_src_op, FALSE);
			break;
	}
}

/*
	This function handles register encoding to code segment.
	The function can be used to encode 2 registers who appear in same instruction line
	by getting is_src_dest=TRUE parameter, otherwise it handles encoding one register
	normally by checking if it's a source or dest with is_src parameter.
*/
void handle_register_encoding(char* src, char* dest, int is_src, int is_src_dest){
	unsigned int word = EMPTY_WORD;
	
	/* If it's a source register. */
	if(is_src){
		unsigned int src_num = 0;
		src_num = (unsigned)atoi(src+1);
		src_num = src_num << SRC_REGISTER_BITS;
		/* If both src and dest are registers we encode them in the same word. */
		if(is_src_dest){
			unsigned int dest_num = 0;
			dest_num = (unsigned)atoi(dest+1);
			dest_num = dest_num << ARE_BITS;
			word = dest_num | src_num;
			encode_in_code_segment(word);
		}
		/* Else the source gets encoded in a word alone. */
		else{
			word = word | src_num;
			encode_in_code_segment(word);
		}
	}
	/* Else its a destination register. */
	else{
		unsigned int dest_num = 0;
		dest_num = (unsigned)atoi(dest+1);
		dest_num = dest_num << ARE_BITS;
		word = word | dest_num;
		encode_in_code_segment(word);
	}	
}

/*
	This function handles encoding label extra word which is the address. 
*/
void handle_label_encoding(char* name, label_ptr label_table){
	int address;
	unsigned int word = EMPTY_WORD;
	label_ptr label = NULL;
	/* If label doesn't exist it's an error. */
	if(!label_exist_check(label_table, name)){
		error_type = LABEL_DOESNT_EXIST;
		throw_error();
		return;
	}
	/* Get label name and address. */
	label = get_label(label_table, name);
	address = get_label_address(label);
	word = (unsigned int)address;
	/* If it's an extern label, then add the current IC to it's addresses so
	   we write them in .ext file at the end, then add EXTERNAL are to the word. */
	if(is_external_label(label)){
		add_ext_label_address(label, IC);
		word = encode_ARE(word, EXTERNAL);
	}
	/* Else we add relocatable are to the word. */
	else{
		word = encode_ARE(word, RELOCATABLE);
	}
	
	encode_in_code_segment(word);
}

/*
	This function returns the number of operands the given command type supports.
*/
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
	return NONE;
}

/*
	This function encodes the ARE bits in the given word and returns it.
*/
unsigned int encode_ARE(unsigned int word, int are){
	word = word << ARE_BITS;
	word = word | are;
	return word;
}






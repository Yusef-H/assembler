#include <stdlib.h>
#include "utilities.h"
#include "first_pass.h"

extern int error_type;

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
int validate_addressing_methods(int command,int first_address_method,int second_address_method){
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


/* This function checks if the given word is a struct operand. */
int is_struct_op(char* word){
	/* This will hold label before the dot (ex: S1.2 -> label = "S1"). */
	char* label = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);
	int length = strlen(word);
	int i = 0;
	if(!label){
		printf("Memory allocation failed");
		exit(1);
	}
	/* take the label name */
	while(i < length && word[i] != '.'){
		label[i] = word[i];
		i++;
	}
	label[i] = '\0';

	/* if theres no dot then its not a struct */
	if(word[i] != '.')
		return FALSE;
	
	/* if its not a label */
	if(!is_label_op(label))
		return FALSE;
	/* skip the dot */
	i++;
	/* struct has two fields only */
	if(word[i] != '1' && word[i] != '2')
		return FALSE;
		
	return TRUE;
}

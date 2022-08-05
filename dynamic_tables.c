/**************************
	Dynamic tables file
**************************/

/*
 	This file contains the implementation of the two dynamic tables:
 	1- Symbols table that is used in the first and second pass.
 	
 	
 	2- Macros table that is used during preprocessing.
 	   - This table is implemented using a linked list for every
 	     macro name, and each macro name will hold its lines as an
 	     inner linked list.
*/

#include "assembler.h"
#include "dynamic_tables.h"
#include "pre_assembler.h"
#include "utilities.h"





/* Add new macro item to the macros linked list */
item_ptr add_new_macro(item_ptr *head_item, char* name){
	item_ptr new_macro;
	item_ptr first,second;
	new_macro = (item_ptr)malloc(sizeof(item_ptr));
	if(!new_macro){
		fprintf(stderr,"Memory Allocation failed");
		exit(0);
	}
/*	 Copy the name of the macro */
	strcpy(new_macro->name, name);
/*	 Initialize lines linked list head to store lines in. */
	new_macro->lines = NULL;
/*	 Insert macro item to the macros linked list. */
	second = *head_item;
	while(second != NULL){
		first = second;
		second = second->next;
	}
/*	 empty */
	if(second == *head_item){
		*head_item = new_macro;
		new_macro->next = NULL;
	}	
	else{
		first->next = new_macro;
		new_macro->next = NULL;
	}
	return new_macro;
}

/* Add a line to the macro item lines linked list. */
void add_macro_line(item_ptr *macro, char* macro_line){
	/* Allocate space for the new line item */
	line_ptr new_line_item = (line_ptr)malloc(sizeof(macro_line)); 
	line_ptr first,second; /* Pointers used to insert new item. */
	/* Allocate space for the item's line field. */
	new_line_item->line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	if(!new_line_item){
		fprintf(stderr,"Memory Allocation failed");
		exit(1);
	}
	
	/* Copy the macro lines inside the new line item field. */
	strcpy((new_line_item->line), macro_line);

	/* Add the new line item to the macro's lines linked list.  */
	second = (*macro)->lines;
	while(second != NULL){
		first = second;
		second = second->next;
	}
	/* If empty: */
	if(second == (*macro)->lines){
		(*macro)->lines = new_line_item;
		((*macro)->lines)->next = NULL;
		
	}	
	else{
		first->next = new_line_item;
		new_line_item->next = NULL;
	}
}

/* Checks if a name (string) exists in the macro table */
item_ptr does_macro_exist(item_ptr macro_head, char* name){
	while(macro_head != NULL){
/*		printf("Macro item name: %s",macro_head->name);*/
		
		if(!strcmp(macro_head->name, name)){
			return macro_head;
		}
		macro_head = macro_head->next;
	}	
	return NULL;
}

void write_macro_lines(item_ptr macro, FILE* fp_am){
	line_ptr temp;
	temp = macro->lines;
	while(temp != NULL){
		fputs((temp)->line, (fp_am));
		temp = (temp)->next;
	}
}

/* Prints the macros linked list and for each macro prints 
   the macro's lines linked list that the macro holds. */
void print_macro_table(item_ptr macro_head){
	while(macro_head){
		printf("\n%s->", macro_head->name);
		/* Printing Macro Lines: */
		printf("(\n");
		while(macro_head->lines){
			printf("%s", (macro_head->lines)->line);
			macro_head->lines = (macro_head->lines)->next; 
		}
		printf("\n)\n");
		/* Done lines list move to next macro. */
		macro_head = macro_head->next;
	}
}




/* Add new label to the labels table */
label_ptr add_label(label_ptr* head, char* name){
	/* Allocate memory for new label item */
	label_ptr new_label_item = (label_ptr)malloc(sizeof(label_item));
	label_ptr temp;
	int name_len = strlen(name);
	/* malloc check */
	if(!new_label_item){
		fprintf(stderr,"Memory Allocation failed");
		exit(1);
	}
	
	/* 
	 Does label name exists check
	 */
	if(name_len < MAX_LABEL_LENGTH){
		new_label_item->label_name = (char*)calloc(sizeof(char),(name_len));
		strcpy(new_label_item->label_name, name);
	}
	else{
		/*error*/
	}

	temp = *head;
	/* if empty */
	if(!temp){
		*head = new_label_item;
		(*head)->next = NULL;
		return *head;
	}

	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = new_label_item;
	new_label_item->next = NULL;
	return new_label_item;
}


void delete_label(label_ptr* head, label_ptr* label){
	label_ptr temp = *head;
	label_ptr holder;
	if(!temp){
		return;
	}
	/* 1 or 2 items cases */
	if(temp->label_name == (*label)->label_name){
		if(temp->next){
			*head = temp->next;
			free(temp);
			return;
		}
		else{
			free(temp);
			return;
		}
	}
	holder = temp;
	temp = temp->next;
	while(temp){
		if(!(strcmp(temp->label_name,(*label)->label_name))){
			holder->next = NULL;
			free(temp);
			return;
		}
		holder = temp;
		temp = temp->next;
	}
	/* Error */
	
}

void print_labels(label_ptr head){
	while(head){
		printf("%s, add:%d, code:%d, ext:%d->",head->label_name, head->address, head->code_flag, head->ext_flag);
		head = head -> next;
	}
	printf(";;");
}








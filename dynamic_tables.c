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
	line_ptr new_line_item = (line_ptr)malloc(sizeof(line_ptr)); 
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

/* Checks if a name (string) exists in the table */
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

void write_macro_lines(item_ptr macro, FILE*** fp_am){
	line_ptr temp;
	temp = macro->lines;
	while(temp != NULL){
		fputs((temp)->line, (**fp_am));
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












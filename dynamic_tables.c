/**************************
	Dynamic tables file
**************************/

/*
 	This file contains the implementation of the two dynamic tables:
 	1- Symbols table that is used in the first and second pass.
 	2- Macros table that is used during preprocessing.
*/

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
	if(first == *head_item){
		*head_item = new_macro;
		new_macro->next = first;
	}	
	/*else{
		first->next = new_macro;
		new_macro->next = NULL;
	}*/
	return new_macro;
}

/* Add a line to the macro item lines linked list. */

void add_macro_line(item_ptr *macro, char* macro_line){
	line_ptr new_line_item = (line_ptr)malloc(sizeof(line_ptr)); 
	line_ptr first,second;
	if(!new_line_item){
		fprintf(stderr,"Memory Allocation failed");
		exit(1);
	}
	strcpy(new_line_item->line, macro_line);
	second = (*macro)->lines;
	
	while(second != NULL){
		first = second;
		second = second->next;
	}
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
	if(macro_head != NULL){
		if(!strcmp(macro_head->name, name)){
			return macro_head;
		}
		macro_head = macro_head->next;
	}	
	return NULL;
}

void print_macro_list(item_ptr macro_head){
	while(macro_head){
		printf("%s->", macro_head->name);
		macro_head = macro_head->next;
	}
}












/**************************
	Dynamic tables file
**************************/

/*
 	This file contains the implementation of the two dynamic tables:
 	1- Symbols table that is used in the first and second pass.
 	2- Macros table that is used during preprocessing.
*/

#include "dynamic_tables.h"

/* Add new macro item to the macros linked list */
void add_new_macro(item_ptr *head_item, char* name){
	item_ptr new_macro;
	item_ptr runner;
	line_ptr lines_head = NULL;
	new_macro = (item_ptr)malloc(sizeof(item_ptr));
	if(!new_macro){
		fprintf(stderr,"Memory Allocation failed");
		exit(1);
	}
	/* Copy the name of the macro */
	strcpy(new_macro->name, name);
	/* Initialize lines linked list head to store lines in. */
	new_macro->lines = lines_head;
	
	/* Insert macro item to the macros linked list. */
	runner = *head_item;
	while(runner->next != NULL){
		/****************
		 Possible error of 2 macros having same name check 
		****************/
		runner = runner->next;
	}
	runner->next = new_macro;	
}

/* Add a line to the macro item lines linked list. */
void add_macro_line(item_ptr *macro, char* macro_line){
	line_ptr new_line_item = (line_ptr)malloc(sizeof(line_ptr)); 
	line_ptr runner;
	if(!new_line_item){
		fprintf(stderr,"Memory Allocation failed");
		exit(1);
	}
	strcpy(new_line_item->line, macro_line);
	runner = (*macro)->lines;
	
	while(runner->next != NULL){
		runner = runner->next;
	}
	runner->next = new_line_item;
}

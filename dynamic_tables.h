/*********************************
	Dynamic tables header file
*********************************/



/*
	This file has all the implementations of the dynamic tables used 
	in the project, which are:
	
	1- Macros linked list: 
		each macro holds an inner linked list 
		for the macro's lines. (used by the pre assembler).
	    
	2- Symbols linked list:
		Each symbol holds the symbols name and it's address and another
		type variable which holds two values (CODE/DATA).
		
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pre assembler DS */
typedef struct macro_line* line_ptr;
typedef struct macro_line{
	char* line;
	line_ptr next;
} macro_line;

typedef struct macro_item* item_ptr;
typedef struct macro_item{
	char name[80];
	line_ptr lines;
	item_ptr next;
} macro_item;


item_ptr add_new_macro(item_ptr *head_item, char* name);
void add_macro_line(item_ptr *macro, char* macro_line);
item_ptr does_macro_exist(item_ptr macro_head, char* name);
void write_macro_lines_to_file(item_ptr*, FILE*);
void print_macro_table(item_ptr);
void write_macro_lines(item_ptr, FILE*);

/* First pass DS */
/*typedef struct label_item label_ptr;
typedef struct label_item{
	char label[MAX_LABEL_LENGTH];
	unsigned int address;
	int are; 
};*/








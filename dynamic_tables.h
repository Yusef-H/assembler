/*********************************
	Dynamic tables header file
*********************************/



/*
 Macros table is built using 2 linked lists this way, one for the macro names,
 and an inner linked list for each macro which holds the lines inside the macro.
 So each macro item holds:
	-The macro name.
	-linked list of the lines inside the macro.
	
*/
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct macro_line* line_ptr;
typedef struct macro_line{
	char line[80];
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
item_ptr does_macro_exist(item_ptr *macro_head, char* name);
void write_macro_lines_to_file(item_ptr*, FILE**);






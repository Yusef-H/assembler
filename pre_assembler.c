/*************************
	Pre Assembler file
*************************/

/*
 This file deals with all the pre assembling process which creates a (.am) file
 from the input file after handling the macros so we can run first pass and 
 second pass on it.
*/

#include "pre_assembler.h"
#include "utilities.h"
#include "assembler.h"
#include "dynamic_tables.h"



/* Pre assembler algorithm (described in project instructions)
   (The tables used for storing macros and macro lines are implemented
    in dynamic_tables.c file).  */
void pre_assembler_algorithm(FILE* fp_as, FILE* fp_am){
	
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char* next_start = line;
	char* word;
	int macro_flag = OFF;
	
	/* Can be changed so we cant see implementation of list!!!!!!!! */
	item_ptr macro_table = NULL;
	item_ptr current_macro;
	item_ptr macro;
	
	while(fgets(line, MAX_LENGTH, fp_as)){
		word = (char*)malloc(sizeof(char)*MAX_LENGTH);
		next_start = get_word(line, word); 
		
		/* If macro name exists in table, then write its lines in the file: */
		if((macro = does_macro_exist(macro_table, word)) ){
			write_macro_lines(macro, fp_am);
		}
		
		/* If its a start of macro, turn macro flag on and prepare for macro
		   name to be added to macro table. */
		else if(!strcmp(word, "macro")){
			macro_flag = ON; 
			
			free(word);
			word = (char*)malloc(sizeof(char)*MAX_LENGTH);
			next_start = get_word(next_start, word); /* Get macro name */
			
			/* Add macro to table and set it as current macro, so all the 
			   following lines until endmacro will be added to its lines
			   in the table. */
			current_macro = add_new_macro(&macro_table, word); 
		}
		else if(!strcmp(word, "endmacro")){
			macro_flag = OFF;
		}
		/* Inside a macro, add lines to the current macro in the table. */
		else if(macro_flag == ON){
			add_macro_line(&current_macro, line);
		}
		/* A normal line and we can write it as is. */
		else{
			fputs(line, (fp_am));
		}
		free(word);
	}
	free(line);
	
	
	/*print_macro_table(macro_table);*/
	
}






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



FILE* pre_assemble(FILE* fp_as, char* filename){
	FILE* fp_am;
	char* appended_file_name;
	appended_file_name = append_filename(filename, AM);
	fp_am = fopen(appended_file_name, "w"); /* Creates a new file */
	
	/* define Macros dynamic table. */
	
	/* run pre assembler algorithm that produces .am file: */
	shledi_pre_assemble(fp_as, fp_am, macros_table_head);	
		
	/* close and return return the .am file. */
	fclose(fp_am);
	return fp_am;
}

void shledi_pre_assemble(FILE* fp_as, FILE* fp_am, item_ptr){
	char line[MAX_LINE_LENGTH];
	char *word;
	int macro_flag = OFF;
	while(fgets(line, MAX_LINE_LENGTH, fp_as)){
		word = get_word(line);
		
		/* add if word exists in macro's table, write it's lines */
		
		
		if(strcmp(word, "macro") == 0){
			macro_flag = ON; 
			/* Add macro name to the macro table */
		}
		if(macro_flag == ON){
			/* Add line to the macro's lines linked list. */
		}
		if(strcmp(word, "endmacro")){
			macro_flag = OFF;
		}
	}
}





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



FILE* pre_assemble(FILE* fp_as, char* filename){
	FILE* fp_am;
	char* appended_file_name;
	appended_file_name = append_filename(filename, AM);
	fp_am = fopen(appended_file_name, "w"); /* Creates a new file */
	
	/* define Macros dynamic table. */
	
	/* run pre assembler algorithm that produces .am file: */
	shledi_pre_assemble(fp_as, &fp_am);	
		
	/* close and return return the .am file. */
	fclose(fp_am);
	return fp_am;
}

void shledi_pre_assemble(FILE* fp_as, FILE** fp_am){
	
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char word[MAX_LENGTH];
	int macro_flag = OFF;
	
	/* Can be changed so we cant see implementation of list!!!!!!!! */
	item_ptr macro_list;
	item_ptr current_macro;
	item_ptr macro;
	while(fgets(line, MAX_LENGTH, fp_as)){
		line_ptr temp;
		int k = 0;
		line = get_word(line, word); 
		
		if(k++ == 1)
			printf("%s",line);
		
		if((macro = does_macro_exist(&macro_list, word)) ){
			if(macro->lines == NULL){
				break;
			}
			temp = macro->lines;
			while(temp != NULL){
				fputs((temp)->line, (*fp_am));
				temp = (temp)->next;
			}
		}
		
		
		else if(strcmp(word, "macro") == 0){
			macro_flag = ON; 
			line = get_word(line, word);
			current_macro = add_new_macro(&macro_list, word);
		}
		else if(strcmp(word, "endmacro")){
			macro_flag = OFF;
		}
		else if(macro_flag == ON){
			add_macro_line(&current_macro, line);
		}
		else{
			fputs(line, (*fp_am));
		}
	}
}





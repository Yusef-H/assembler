/************************
	Assembler File
************************/

/*
	This file handles opening the file and running the 
	first and second pass on it
*/

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
#include "pre_assembler.h"
/*#include "first_pass.h"
#include "second_pass.h"*/
#include "utilities.h"

/* Global Variable for error types in all the program
   The error types are in an enum in assembler.h file */
int error_type;

/* Code segment and Data segment arrays in order to split them
   in the object file. */
unsigned int data_segment[SIZE];
unsigned int code_segment[SIZE];
int IC = 0;


/* The unique base 32 we got in the instructions. */
/*const char base_32[32] = {
	'!', '@', '#', '$', '%', '^', '&', '^', '<', '>',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'
};
*/


/* 
		foreach file in argv[1...argc-1]:
		
			***Adds .s to file name
			***Opens file
			***Run pre_assembler on the .s file and create .am file.
			Run first pass on .am file
			Run second pass on .am file create ob,ent,ext files.
			Close file
			
	*/

int main(int argc, char *argv[]){
	FILE *fp_as; /* input file (.as) */
	FILE *fp_am;
	char* appended_filename;
	int i;
	
	for(i = 1; i<argc; i++){
		appended_filename = append_filename(argv[i], AS); 
		
		fp_as = fopen(appended_filename, "r");
		if(!fp_as){
			error_type = OPEN_FILE;
			throw_error(0);
		}
		
		
		fp_am = pre_assemble(fp_as, argv[i]); /*Creates .am file after
											    handling macros*/
		
		printf("\nPre assembling file *%s* done!\n",appended_filename);
		
		fclose(fp_as);  /* Done with the input file. */
		
		/* First pass on fp_am */
		
		/* Second pass on fp_am */
		
		
	
	
		/*free(appended_filename);	*/
	}
	
	return 0;
}





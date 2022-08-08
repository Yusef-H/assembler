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
#include "second_pass.h"
#include "utilities.h"

#include "first_pass.h"
/* Global Variable for error types in all the program
   The error types are in an enum in assembler.h file */
int error_type;

/* Code segment and Data segment arrays in order to split them
   in the object file. */
unsigned int data_segment[SIZE];
unsigned int code_segment[SIZE];

/* directives */
const char *directives[NUM_DIRECTIVES] = { 
	".data", ".string", ".struct", ".entry", ".extern"
};

/* The unique base 32 we got in the instructions. */
const char base_32[32] = {
	'!', '@', '#', '$', '%', '^', '&', '^', '<', '>',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'
};

/* Instruction commands */
const char *commands[NUM_COMMANDS] = { 
	"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
	"dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"
};

/* registers */
const char *registers[NUM_REGISTERS] = {
	"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};




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
	FILE *fp_am; /* file after pre assembling. */
	label_ptr labels_table; /* will hold the labels table returned from first_pass. */
	char* appended_filename_as;
	char* appended_filename_am;
	int i;
	
	for(i = 1; i<argc; i++){
		appended_filename_as = append_filename(argv[i], AS); 
		appended_filename_am = append_filename(argv[i], AM);
		
		fp_as = fopen(appended_filename_as, "r");
		if(!fp_as){
			error_type = OPEN_FILE;
			throw_error(0);
		}
		
		fp_am = fopen(appended_filename_am, "w+");
		if(!fp_am){
			error_type = OPEN_FILE;
			throw_error(0);
		}
		
		/* Run pre assembler */
		pre_assembler_algorithm(fp_as, fp_am); 
		
		fclose(fp_as);  /* Done with the input file. */
		
		
		/* First pass on fp_am (input file after handling macros). */
		rewind(fp_am); /* rewind pointer to start of file. */
		labels_table = first_pass(fp_am);
		
		/* Second pass on fp_am */
		rewind(fp_am);
		second_pass(fp_am, labels_table);
		
	
	
		/*free(appended_filename);	*/
	}
	
	return 0;
}





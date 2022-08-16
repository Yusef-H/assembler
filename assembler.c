/*******************************
	Assembler File
********************************/

/*
	This file handles opening the files and running the 
	first and second pass on them and
*/

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
#include "utilities.h"
#include "first_pass.h"
#include "second_pass.h"

/* Global Variable for error types in all the program
   The error types are defined in an enum in assembler.h file */
int error_type;
int error_occurred = FALSE;

/* Code segment and Data segment arrays in order to split them
   in the object file. */
unsigned int data_segment[SIZE];
unsigned int code_segment[SIZE];
/* Instruction and Data counters used in first & second pass. */
int IC;
int DC;

/* directives */
const char *directives[NUM_DIRECTIVES] = { 
	".data", ".string", ".struct", ".entry", ".extern"
};

/* The unique base 32 we got in the instructions. */
const char base_32[32] = {
	'!', '@', '#', '$', '%', '^', '&', '*', '<', '>',
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
	This function handles the main activities in the assembler program:
	Runs pre assembler then first & second passes on each file we got in
	the command line.
 */
int main(int argc, char *argv[]){
	FILE *fp_as; /* input file (.as) */
	FILE *fp_am; /* file after pre assembling. (.am) */
	label_ptr labels_table; /* will hold the labels table returned from first_pass. */
	char* appended_filename_as;
	char* appended_filename_am;
	int i;
	for(i = 1; i<argc; i++){
		/* Get the .as file name */
		appended_filename_as = append_filename(argv[i], AS); 
		/* Prepare the .am file name for pre assembling. */
		appended_filename_am = append_filename(argv[i], AM);
		
		fp_as = fopen(appended_filename_as, "r");
		fp_am = fopen(appended_filename_am, "w+");
		if(!fp_am || !fp_am){
			/* fp_am file opening can fail also if there's permission
			   issues
		    */
			printf("Cannot open file");
			/* Continue if there's other files */
			continue;
		}
		
		/* Run pre assembler and create the .am file. */
		pre_assembler_algorithm(fp_as, fp_am); 
		fclose(fp_as);  /* Done with the .as file. */
		
		/* Run First pass on fp_am (.as file after handling macros). */
		rewind(fp_am); /* rewind pointer to the start of file. */
		
		/* first pass returns the label table so we use it in second pass. */
		labels_table = first_pass(fp_am);
	
		/* Second pass on fp_am */
		rewind(fp_am);
		second_pass(fp_am, labels_table, argv[i]);
	
		/* 
		Free the used memory and reset globals so we can move on 
		the new file (if there is).
		*/
		
		labels_table = free_labels_table(&labels_table);
		free(appended_filename_as);
		free(appended_filename_am);	
		reset_globals();
	}
	
	return 0;
}

/* This function resets the global variables and arrays used
   in the program. */
void reset_globals(){
	int i;
	/* Clear code segment. */
	for(i = 0; i< IC; i++){
		code_segment[i] = 0;
	}
	/* Clear data segment. */
	for(i = 0; i< DC; i++){
		data_segment[i] = 0;
	}
	/* reset errors global variables. */
	error_type = NO_ERROR;
	error_occurred = FALSE;
}





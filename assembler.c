/*******************************
	Assembler File
********************************/

/*
	This file handles opening the files and running the 
	first and second pass on them and
*/

#include <stdio.h>
#include <stdlib.h>
#include "pre_assembler.h"
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
		second_pass(fp_am, labels_table, argv[i]);
		
		
		labels_table = free_labels_table(&labels_table);
		
		/* Reset global variables after dealing with each file. */
		reset_globals();
		free(appended_filename_as);
		free(appended_filename_am);	
	}
	
	return 0;
}

/* This function resets the global variables and arrays used
   in the program. */
void reset_globals(){
	int i;
	for(i = 0; i< IC; i++){
		code_segment[i] = 0;
	}
	for(i = 0; i< DC; i++){
		data_segment[i] = 0;
	}
	IC = 0;
	DC = 0;
	error_type = NO_ERROR;
	error_occurred = FALSE;
}





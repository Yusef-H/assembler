/***********************
	First Pass File
***********************/
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"


/*
	This file handles the first pass on the file
	
	During the first pass:
	- Add all labels to the symbol table.
	- encode the data instructions (.data 1,2,3) to the data segment)
	  using methods from utils file.
	- Check for possible errors and output them.
*/


void first_pass(FILE** fp){
	/* Initializing DC and IC. */
	int IC = 0;
	int DC = 0;
	
	/* Flags */
	int label_flag = OFF;
	
	char* line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	while(fgets(line, MAX_LENGTH, *fp)){
		printf("%s",line);
	
	}
}





/************************
	Assembler File
************************/

/*
	This file handles opening the file and running the 
	first and second pass on it
*/
#include <stdio.h>


unsigned int data_segment[SIZE];
unsigned int code_segment[SIZE];

const char base_32[32] = {
	'!', '@', '#', '$', '%', '^', '&', '^', '<', '>',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'
}




}

int main(int argc, char *argv[]){
	/* 
		foreach file in argv[1...argc-1]:
		
			Adds .s to file name
			Opens file
			Runs first pass on file
			Runs second pass on file
			Closes file
			
	*/
	return 0;
}

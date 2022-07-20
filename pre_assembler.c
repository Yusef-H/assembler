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
	/*
		-define Macros dynamic table.
		-run pre assembler algorithm (function) that returns .am file.
		-return the .am file.
	*/
	return fp_am;
}

/*********************************************
		First Pass header file
**********************************************/

/* Include dynamic tables header so we can use label types. */
#include "dynamic_tables.h"

/* General functions that are in the first_pass.c file */
label_ptr first_pass(FILE*);
void parse_line(char*,label_ptr*);
void directive_handler(int, char*, label_ptr*);
void command_handler(int, char*);
int is_directive(char*);
int is_command(char*);

/* Instructions functions (in first_pass_instructions.c file). */
int validate_num_operands(int, int, int);
int validate_addressing_methods(int, int, int);
unsigned int create_first_word(int,int , int , int , int);
void encode_in_code_segment(unsigned int);
int address_method_detector(char*);
int method_extra_words(int);

/* Directives functions (in first_pass_directives.c file).*/
void data_directive_handler(char*);
void string_directive_handler(char*);
void struct_directive_handler(char*);
void entry_directive_handler(char*);
void extern_directive_handler(char*, label_ptr*);
void encode_in_data_segment(int);



/* Include dynamic tables header so we can use label types. */
#include "dynamic_tables.h"

void first_pass(FILE*);
void parse_line(char*,label_ptr*);
int is_label(char*, char*);
int is_directive(char*);
int is_command(char*);
int is_label_op(char*);
int is_struct_op(char*);

int validate_num_operands(int, int, int);
int validate_addressing_methods(int, int, int);

void directive_handler(int, char*, label_ptr*);
void data_directive_handler(char*);
void string_directive_handler(char*);
void struct_directive_handler(char*);
void entry_directive_handler(char*);
void extern_directive_handler(char*, label_ptr*);
void encode_in_data_segment(int value);

unsigned int create_first_word(int,int , int , int , int);
void encode_in_code_segment(unsigned int);

void command_handler(int command, char* params);
int address_method_detector(char* operand);
int method_extra_words(int method);

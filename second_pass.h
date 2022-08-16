/****************************************
		Second Pass Header file
****************************************/
#include "dynamic_tables.h"

/* Second pass functions. */
void second_pass(FILE*, label_ptr, char*);
void second_parse_line(char*, label_ptr);
void second_pass_command_handler(int, char*, label_ptr);
int command_num_operands(int);
void encode_extra_words(int, int, int, char*, char*, label_ptr);
void encode_op_word(char*, int, int,label_ptr);
void handle_label_encoding(char*,label_ptr);
void handle_register_encoding(char*, char*, int, int);
unsigned int encode_ARE(unsigned int, int);
void output_files_handler(char*,label_ptr,int,int);
void create_object_file(char*);
void create_ext_file(char*,label_ptr);
void create_ent_file(char*,label_ptr);

#include "dynamic_tables.h"
void second_pass(FILE* fp_am, label_ptr);
void second_parse_line(char*, label_ptr);

void second_pass_command_handler(int command, char* params, label_ptr);
int command_num_operands(int command);
void encode_extra_words(int num_operands, int, int, char*, char*, label_ptr);
void encode_op_word(char* operand, int method, int is_src_op,label_ptr);
void handle_label_encoding(char*,label_ptr);

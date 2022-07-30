#include <ctype.h>
#include "dynamic_tables.h"

void first_pass(FILE*);
void parse_line(char*,label_ptr*);
int is_label(char*, char*);
int is_directive(char*);

void directive_handler(int, char*);
void data_directive_handler(char*);
void string_directive_handler(char*);
void struct_directive_handler(char*);

char* before_data_param(char* params);
char* after_data_param(char* params);

void encode_in_data_segment(int value);

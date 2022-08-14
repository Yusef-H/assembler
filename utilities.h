
/* Throws error according to error type. */
void throw_error();

/* Appends file names with the suffixes according to the type (.as,.am,.ob,..).  */
char* append_filename(char* filename, int append_type);

/* gets the next word in line */
char* get_word(char*, char*);

void empty_word(char*);

int is_reserved_word(char*);

int is_label(char* word, char* label_name);
int is_label_op(char* word);
int is_struct_op(char* word);


char* skip_whitespaces(char* );

char* skip_line(char*);

int is_end_of_line(char c);

int is_register(char*);

char* convert_to_base32(unsigned int word);

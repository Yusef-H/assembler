
/* Throws error according to error type. */
void throw_error();

/* Appends file names with the suffixes according to the type (.as,.am,.ob,..).  */
char* append_filename(char* filename, int append_type);

/* gets the next word in line */
char* get_word(char*, char*);

void empty_word(char*);

int is_reserved_word(char*);

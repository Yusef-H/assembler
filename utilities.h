
/* Throws error according to error type. */
void throw_error(int line_number);

/* Appends file names with the suffixes according to the type (.as,.am,.ob,..).  */
char* append_filename(char* filename, int append_type);

/* gets the next word in line */
char* get_word(char*);

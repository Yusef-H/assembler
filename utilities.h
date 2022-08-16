/***********************************************
	Utilities header file
***********************************************/

/* handles all errors output */
void throw_error();

/* Converts a 10 bit word to 2 digits number in base32. */
char* convert_to_base32(unsigned int);

/* This function appends filenames (.as, .am, .ent, .ext) */
char* append_filename(char*, int);

/* Very specific functions that are used to check strings and analyze them
   or parse line helpers. */
int is_reserved_word(char*);
int is_label(char*, char*);
int is_label_op(char*);
int is_struct_op(char*);
int is_end_of_line(char);
int is_register(char*);
char* get_word(char*, char*);
char* skip_whitespaces(char* );



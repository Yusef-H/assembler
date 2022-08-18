/*********************************
	Dynamic tables header file
*********************************/

#ifndef DYNAMIC_TABLES_H

#define DYNAMIC_TABLES_H

/*** Pre Assembler Data Structures ***/

/* Lines linked list for each macro item. */
typedef struct macro_line* line_ptr;
typedef struct macro_line{
	char* line;
	line_ptr next;
} macro_line;

/* Macros Linked list that each macro item holds its lines as an
   inner lines linked list.  */
typedef struct macro_item* item_ptr;
typedef struct macro_item{
	char name[80];
	line_ptr lines;
	item_ptr next;
} macro_item;

/* Pre Assembler Linked lists functions. */
item_ptr add_new_macro(item_ptr*, char*);
void add_macro_line(item_ptr*, char*);
item_ptr does_macro_exist(item_ptr, char*);
void write_macro_lines_to_file(item_ptr*, FILE*);
void print_macro_table(item_ptr);
void write_macro_lines(item_ptr, FILE*);


/*** First & Second Pass Data Structures ***/

/* Linked list of addresses for external labels. */
typedef struct address_item* address_ptr;
typedef struct address_item{
	int address;
	address_ptr next;
} address_item;

/*
   Labels linked list where each label item holds:
   - The label name.
   - The label address.
   - code_flag: Which is true if the label is in instruction line otherwise false.
   - ext_flag: to mark extern labels.
   - addresses linked list: for extern labels (used to create .ext file).
   - ent_flag: to mark entry labels.
*/
typedef struct label_item* label_ptr;
typedef struct label_item{
	char* label_name;
	int address;
	int code_flag; 
	int ext_flag;
	address_ptr addresses;
	int ent_flag;
	label_ptr next;
} label_item;

/*** First & Second Pass Lists functions ***/

label_ptr add_label(label_ptr*, char*);
void delete_label(label_ptr*,label_ptr*);
void set_label_address(label_ptr,int);
label_ptr get_label(label_ptr, char*);
int get_label_address(label_ptr);
void turn_label_code_flag(label_ptr);
void turn_label_ext_flag(label_ptr);
int turn_label_ent_flag(label_ptr, char*);
void print_labels(label_ptr);
void update_addresses(label_ptr, int);

void update_ext_addresses(label_ptr);
int label_exist_check(label_ptr, char*);
int is_external_label(label_ptr);
int ent_exist_check(label_ptr);
int ext_exist_check(label_ptr);
void add_ext_label_address(label_ptr, int);
label_ptr free_labels_table(label_ptr*);



#endif





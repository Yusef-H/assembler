/**************************
	Dynamic tables file
**************************/

/*
 	This file contains the implementation of the two dynamic tables:
 	1- Labels table that is used in the first and second pass.
 		- This is implemented as a linked list where each node holds 
 		  extra information and tags that tell us information about the
 		  label like is it extern,entry, in code segment or data segment..
 		  And each extern label holds an addresses inner linked list which
 		  holds the addresses the extern label was used in so we can write
 		  them in .ext file after second pass.
 	
 	2- Macros table that is used during preprocessing.
 	   - This table is implemented using a linked list for every
 	     macro name, and each macro name will hold its lines as an
 	     inner linked list.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "dynamic_tables.h"
#include "utilities.h"


/***************************************************************/
				/* Pre Assembler Dynamic Tables  */
/***************************************************************/

/* Add new macro item to the macros linked list */
item_ptr add_new_macro(item_ptr *head_item, char* name){
	item_ptr new_macro = (item_ptr)malloc(sizeof(item_ptr));
	item_ptr temp;
	if(!new_macro){
		printf("Memory Allocation failed");
		exit(EXIT_FAILURE);
	}
	/* Copy the name of the macro */
	strcpy(new_macro->name, name);
	/* Initialize lines linked list head to store lines in. */
	new_macro->lines = NULL;
	/* Insert macro item to the macros linked list. */
	temp = *head_item;
	if(temp == NULL){
		*head_item = new_macro;
		new_macro->next = NULL;
		return new_macro;
	}	
	while(temp -> next){
		temp = temp->next;
	}
	temp->next = new_macro;
	new_macro->next = NULL;
	
	return new_macro;
}

/* Add a line to the macro item lines linked list. */
void add_macro_line(item_ptr *macro, char* macro_line){
	/* Allocate space for the new line item */
	line_ptr new_line_item = (line_ptr)malloc(sizeof(macro_line)); 
	line_ptr first,second; /* Pointers used to insert new item. */
	/* Allocate space for the item's line field. */
	new_line_item->line = (char*)malloc(sizeof(char)*MAX_LENGTH);
	if(!new_line_item || !(new_line_item->line)){
		printf("Memory Allocation failed");
		exit(EXIT_FAILURE);
	}
	
	/* Copy the macro lines inside the new line item field. */
	strcpy((new_line_item->line), macro_line);
	/* Add the new line item to the macro's lines linked list.  */
	second = (*macro)->lines;
	while(second){
		first = second;
		second = second->next;
	}
	/* If empty: */
	if(second == (*macro)->lines){
		(*macro)->lines = new_line_item;
		((*macro)->lines)->next = NULL;
		
	}	
	else{
		first->next = new_line_item;
		new_line_item->next = NULL;
	}
}

/* Checks if a name (string) exists in the macro table as a macro name. */
item_ptr does_macro_exist(item_ptr macro_head, char* name){
	while(macro_head != NULL){
		if(!strcmp(macro_head->name, name)){
			return macro_head;
		}
		macro_head = macro_head->next;
	}	
	return NULL;
}

/* This function writes the macro lines list into the fp_am file. */
void write_macro_lines(item_ptr macro, FILE* fp_am){
	line_ptr temp;
	temp = macro->lines;
	while(temp != NULL){
		fputs((temp)->line, (fp_am));
		temp = (temp)->next;
	}
}

/* Prints the macros linked list and for each macro prints 
   the macro's lines linked list that the macro holds. */
void print_macro_table(item_ptr macro_head){
	while(macro_head){
		printf("\n%s->", macro_head->name);
		/* Printing Macro Lines: */
		printf("(\n");
		while(macro_head->lines){
			printf("%s", (macro_head->lines)->line);
			macro_head->lines = (macro_head->lines)->next; 
		}
		printf("\n)\n");
		/* Done lines list move to next macro. */
		macro_head = macro_head->next;
	}
}

/***************************************************************/
				/* First pass Dynamic tables  */
/***************************************************************/


/* This functions adds a new label to the labels table */
label_ptr add_label(label_ptr* head, char* name){
	/* Allocate memory for new label item */
	label_ptr new_label_item = (label_ptr)malloc(sizeof(label_item));
	label_ptr temp;
	int name_len = strlen(name);
	/* malloc check */
	if(!new_label_item){
		printf("Memory Allocation failed");
		exit(EXIT_FAILURE);
	}
	/* initialize label fields. */
	new_label_item -> addresses = NULL;
	new_label_item->label_name = (char*)calloc(sizeof(char),(name_len));
	strcpy(new_label_item->label_name, name);
	new_label_item->ent_flag = OFF;
	new_label_item->ext_flag = OFF;

	/* Insert the label at the end of the linked list. */
	temp = *head;
	/* If the list is empty. */
	if(!temp){
		*head = new_label_item;
		(*head)->next = NULL;
		return *head;
	}
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = new_label_item;
	new_label_item->next = NULL;
	return new_label_item;
}

/* This function deletes a label from the label table. */
void delete_label(label_ptr* head, label_ptr* label){
	label_ptr temp = *head;
	label_ptr holder;
	/* If empty list. */
	if(!temp){
		return;
	}
	/* 1 or 2 items cases */
	if(temp->label_name == (*label)->label_name){
		if(temp->next){
			*head = temp->next;
			free(temp);
			return;
		}
		else{
			free(temp);
			*head = NULL;
			return;
		}
	}
	holder = temp;
	temp = temp->next;
	while(temp){
		if(!(strcmp(temp->label_name,(*label)->label_name))){
			holder->next = NULL;
			free(temp);
			return;
		}
		holder = temp;
		temp = temp->next;
	}
}

/* This function updates the labels addresses after the first pass. */
void update_addresses(label_ptr head, int ic){
	while(head){
		/* skip the labels of instructions */
		if(!(head->code_flag) && !(head->ext_flag)){
			head->address = head->address + ic  + MEMORY_START;
			head = head->next;
		}
		else if((head->code_flag) && !(head->ext_flag)){
			head->address = head->address + MEMORY_START;
			head = head->next;
		}
		else{
			/* we skip external addresses for now
			   they will be updated after second pass. */
			head = head->next;
		}
		
	}
	
}

/* This function updates the external label addresses after the second pass. */
void update_ext_addresses(label_ptr head){
	while(head){
		if(head->ext_flag){
			address_ptr temp = head->addresses;
			while(temp){
				temp->address = temp->address + MEMORY_START;
				temp = temp->next;
			}
		}
		head = head->next;
	}
}

/* 
	This function adds an address to an extern label item inner addresses
	linked list.
*/
void add_ext_label_address(label_ptr label, int address){
	/* Allocate memory for new address item. */
	address_ptr address_item = (address_ptr)malloc(sizeof(address_item));
	address_ptr temp; /* Will be used to run on addresses linked list. */
	if(!address_item){
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	
	address_item->address = address; 
	temp = label->addresses;
	
	/* Add new address item at the end of the list. */
	if(!temp){	
		label->addresses = address_item;
		address_item->next = NULL;
		return;
	}
	while(temp->next){
		temp = temp->next;
	}
	temp->next = address_item;
	address_item->next = NULL;
}

/*
	This function handles freeing the memory allocated for the labels 
	linked list items, and also frees the inner addresses lists for 
	extern labels.
*/
label_ptr free_labels_table(label_ptr* head_addr){
	label_ptr head = *head_addr;
	label_ptr label_to_free;
	while(head){
		label_to_free = head;
		/* If it's extern label, free it's inner addresses list. */
		if(label_to_free->ext_flag){
			address_ptr address_head = label_to_free->addresses;
			address_ptr address_to_free;
			while(address_head){
				address_to_free = address_head;
				address_head = address_head->next;
				free(address_to_free);
			}
			address_head = NULL;
		}
		head = head->next;
		free(label_to_free);
	}
	head = NULL;
	return head;
}

/* These two functions check if we ever got an extern/entry label. */

int ext_exist_check(label_ptr head){
	while(head){
		if(head->ext_flag)
			return TRUE;
		head = head->next;
	}
	return FALSE;
}
int ent_exist_check(label_ptr head){
	while(head){
		if(head->ent_flag)
			return TRUE;
		head = head->next;
	}
	return FALSE;
}

/* This function returns label item that has the given name and null if
   it doesn't exist in the linked list. */
label_ptr get_label(label_ptr head, char* name){
	while(head){
		if(strcmp(head->label_name, name) == 0){
			return head;
		}
		head = head->next;
	}
	return NULL;
}

/* Functions used to abstract accessing labels table: */

void set_label_address(label_ptr label, int address){
	(*label).address = address;
}
void turn_label_code_flag(label_ptr label){
	(*label).code_flag = ON;
}
void turn_label_ext_flag(label_ptr label){
	(*label).ext_flag = ON;
}

int is_external_label(label_ptr label){
	return label->ext_flag;
}

int get_label_address(label_ptr label){
	return label->address;
}
int turn_label_ent_flag(label_ptr head, char* label_name){
	while(head){
		if(strcmp(head->label_name, label_name) == 0){
			head->ent_flag = ON;
			return TRUE;
		}
		head = head->next;
	}
	return FALSE;
}

/*
	This function checks if a name exists as a label name in the table.
*/
int label_exist_check(label_ptr head,char* name){
	while(head){
		if(strcmp(head->label_name,name) == 0)
			return TRUE;
		head = head->next;
	}
	return FALSE;
}
/*
	This function prints the label table with some of its components.
*/
void print_labels(label_ptr head){
	while(head){
		printf("%s    add:%d, code:%d, ent:%d, ext:%d",head->label_name, head->address, head->code_flag, head->ent_flag,head->ext_flag);
		if(head->ext_flag){
			address_ptr temp = head->addresses;
			printf("      external addresses:");
			while(temp){
				printf(" %d  ",temp->address);
				temp = temp->next;
			}
		}
		printf("\n\n");
		head = head -> next;
	}
	printf(";;");
}











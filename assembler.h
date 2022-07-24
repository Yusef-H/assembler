#ifndef ASSEMBLER_H

#define ASSEMBLER_H

/* Bounded Sizes */
#define SIZE 1000
#define MAX_LENGTH 81 /* extra one for '\n' */
#define MAX_LABEL_LENGTH 30

/* constants */
#define NUM_DIRECTIVES 5


/* file types */
enum file_types{AS, AM, OB, EXT, ENT};

/* error types */
enum error_types{OPEN_FILE};

/* Directives */
enum directive_types{NOT_DIRECTIVE, DATA, STRING, STRUCT, ENTRY, EXTERN};

/* word types */
typedef struct {
	unsigned int bits:10;
} word;

/* Flags */
#define OFF 0
#define ON 1
/* Boolean Values */
#define TRUE 1
#define FALSE 0


#endif



/* Bounded Sizes */
#define SIZE 1000
#define MAX_LENGTH 81 /* extra one for '\n' */
#define MAX_LABEL_LENGTH 32


/* file types */
enum file_types{AS, AM, OB, EXT, ENT};

/* error types */
enum error_types{OPEN_FILE};

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


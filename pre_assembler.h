#include <stdio.h>
FILE* pre_assemble(FILE* fp_as, char* filename);
void pre_assemble_algorithm(FILE*, FILE**);
int check_for_macros(FILE**);

#define TRUE 1
#define FALSE 0

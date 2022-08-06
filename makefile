assembler: dynamic_tables.o pre_assembler.o utilities.o assembler.o first_pass.o first_pass_directives.o first_pass_instructions.o
	gcc -g -Wall -pedantic -ansi dynamic_tables.o pre_assembler.o utilities.o assembler.o first_pass.o first_pass_directives.o first_pass_instructions.o -o assembler
dynamic_tables.o: dynamic_tables.c dynamic_tables.h
	gcc -c -g -Wall -pedantic -ansi dynamic_tables.c
pre_assembler.o: pre_assembler.c utilities.h assembler.h
	gcc -c -g -Wall -pedantic -ansi pre_assembler.c
utilities.o: utilities.c utilities.h assembler.h
	gcc -c -g -Wall -pedantic -ansi utilities.c
assembler.o: assembler.c assembler.h pre_assembler.h utilities.h
	gcc -c -g -Wall -pedantic -ansi assembler.c
first_pass.o: first_pass.c assembler.h pre_assembler.h utilities.h
	gcc -c -g -Wall -pedantic -ansi first_pass.c
first_pass_directives.o: first_pass_directives.c first_pass.h assembler.h utilities.h
	gcc -c -g -Wall -pedantic -ansi first_pass_directives.c
first_pass_instructions.o: first_pass_instructions.c first_pass.h assembler.h utilities.h
	gcc -c -g -Wall -pedantic -ansi first_pass_instructions.c

	;undefined labels
A:		mov hellothere, r1
		sub A, abc
		bne imNotADefinedLabel
	

	;illegal label definition
thislabelnameistoolongSoItsNotAllowed: cmp r1,r3
InvalidChars;,;,;;h;;;:	bne A
123AlphabetStart: mov r2,r3
wheresMyColon	bne A	

	;entry and extern
		.entry undefinedLabel
		.extern s;s
		.entry A,IllegalExtraParam
		.extern A,IllegalExtraParam 
		.entry
		.extern

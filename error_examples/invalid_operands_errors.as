	;undefined labels
A:		mov hellothere, r1
		sub A, abc
		bne imNotADefinedLabel
		
	; number operands and extra text
		.struct 13
		.struct 15,
		.struct 16, notastring
		add #1
		add #1 , A  hello this is extra text
		.string 1,2,34
		.string "1231
		.string "12", "12", "abc"
		.struct
		bne A, r2
		mov r2
		cmp r1
		
	;random invalid operands
		add # , r4
		sub #dd213dbr, A
		add A;,.,no, r1
		bne A.3
		.struct 55, "

		
	; random errors with commas
		.data 1,2,3 , , 3
		.data 1, +3, -4, ,,,,,,, 4
		.data, 1
		add, r2, r3
		bne, A
		.data 1  3  -4
		.data 1, + , 2
		.data +1, -, +, 22
		.data 1, 2+3
		.data abc, "17", 5
		.string, "abc"
		cmp, r2,r3
		
	;illegal label definition
thislabelnameistoolongSoItsNaaaa: cmp r1,r2
		
		
	
	
	
	
	

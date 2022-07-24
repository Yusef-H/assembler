MAIN: 	mov r0,r1
     	cmp #-5,K
     	cmp #2,S.1
      	sub S.2,W
      	not K
LOOP:   jmp R
r7:     jmp R
		macro m1
		prn K
		inc r7
		endmacro
		macro m6
		cmp #-6,r1
		not K
		endmacro
		jsr S.2
		m6
		red W
		m1
K: 		.data -5
		m1
S: 		.struct 2,"ab"
		m6
		

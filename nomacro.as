MAIN: 	mov r0,r1
     	cmp #-5,K
     	cmp #2,S.1
      	sub S.2,W
      	not K
LOOP:   jmp R
		jsr S.2
		red W
K: 		.data -5
S: 		.struct 2,"ab"
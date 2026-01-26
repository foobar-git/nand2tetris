/// WORK IN PROGRESS
//====================


// DRAWING ON SCREEN

//address = SCREEN
//n = RAM[0]
//i = 0

//LOOP:
//	if i > n goto END
//	RAM[address] = -1	// 16 x black pixels
//	// advances to the next row:
//	address = address + 32
//	i = i + 1
//	goto LOOP
//	
//END:
//	goto END
	
// =====================

@SCREEN
D=A
@address
M=D			// address = 16384
			// SCREEN'S address

@R0
D=M
@n
M=D			// storing n from RAM[0]

@i
M=0

(LOOP)
	@i
	D=M
	@n
	D=D-M
	@END	// if i > n goto END
	D;JGT	// ie. if i - n > 0

	@address
	M=-1
	// ...
	
(END)
	@END
	0;JMP
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.


// R0		Multiplicant, the number we keep adding how many times
// R1		How many times to add
// R2		Accmulator, sum total starting at 0
// count	Counter, every time we add to R2, we subtract -1 from count

// Initialization of variables
	@R2
	M=0
	@R1
	D=M
	@count
	M=D

(LOOP)
	@count
	D=M			// Get current counter value
	@END		// If count <= 0, we are done, jump to END label
	D;JLE

	@R0
	D=M			// Get value to add
	@R2
	M=D+M		// Adding R0 to R2

	@count
	M=M-1		// Decrease counter
	@LOOP		// Go to LOOP label (repeat until done)
	0;JMP
	
(END)
	@END
	0;JMP

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.

	@color		// Color black (-1 is black, 0 is white)
	M=-1

(CONT_LOOP)
	// Setting address to the start of the screen
	@SCREEN
	D=A
	@address
	M=D					// 16384
	
	// If keyboard is being pressed
	@KBD
	D=M
	@DRAW_BLACK
	D;JNE
	
(DRAW_WHITE)
	@color
	M=0
	@DRAW_LOOP
	0;JMP
	
(DRAW_BLACK)
	@color
	M=-1

(DRAW_LOOP)
	@color
	D=M
	@address
	A=M           	// Get value stored in address
    M=D           	// RAM[address] = color
	@address
	M=M+1			// Move to next pixel chunk
	
	// Check if we hit the end of the screen
    D=M
    @24576        	// End of screen
    D=D-A
    @CONT_LOOP
    D;JEQ         	// If screen is full, go back to check KBD again

    @DRAW_LOOP
    0;JMP         	// Otherwise, keep painting pixels
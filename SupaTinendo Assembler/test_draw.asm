/// WORK IN PROGRESS
//====================

// Draws a filled rectangle at the screen's top left corner, with
// width of 16 pixels and height of RAM[0] pixels.
// Usage: put a non-negative number (rectangle's height) in RAM[0]

// Initialize height from R0
    @R0
    D=M
    @n
    M=D         // n = height

// Initialize address to top-left of screen
    @SCREEN
    D=A
    @address
    M=D         // addr = 16384

(LOOP)
    // Check if n > 0
    @n
    D=M
    @END
    D;JLE       // If n <= 0, we are done

    // Drawing the 16-pixel bar (how many times to make a rectangle)
    @address
    A=M         // Get the address we stored in 'address'
    M=-1        // Set all 16 pixels to black (1111111111111111)

    // Move to the next row (32 words away)
    @32
    D=A
    @address
    M=D+M       // address = address + 32 (32 wraps around)

	// Decrement counter
    @n
    M=M-1
    @LOOP
    0;JMP

(END)
    @END
    0;JMP
	
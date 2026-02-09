// --- BOOTSTRAP ---
@256
D=A
@SP
M=D
// VM: 	push argument 1         // sets THAT, the base address of the
@ARG
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 1           // that segment, to argument[1]
@SP
AM=M-1
D=M
@4
M=D

// VM: 	push constant 0         // sets the series' first and second
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop that 0              // elements to 0 and 1, respectively       
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: 	push constant 1   
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop that 1              
@THAT
D=M
@1
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: 	push argument 0         // sets n, the number of remaining elements
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 2         // to be computed, to argument[0] minus 2,
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	sub                     // since 2 elements were already computed.
@SP
AM=M-1
D=M
A=A-1
M=M-D

// VM: 	pop argument 0          
@ARG
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: label LOOP
(FibonacciSeries$LOOP)

// VM: 	push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	if-goto COMPUTE_ELEMENT // if n > 0, goto COMPUTE_ELEMENT
@SP
AM=M-1
D=M
@FibonacciSeries$COMPUTE_ELEMENT
D;JNE

// VM: 	goto END                // otherwise, goto END
@FibonacciSeries$END
0;JMP

// VM: label COMPUTE_ELEMENT
(FibonacciSeries$COMPUTE_ELEMENT)

// VM: 	push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push that 1
@THAT
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: 	pop that 2
@THAT
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: 	push pointer 1
@4
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: 	pop pointer 1 
@SP
AM=M-1
D=M
@4
M=D

// VM: 	push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

// VM: 	pop argument 0          
@ARG
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: 	goto LOOP
@FibonacciSeries$LOOP
0;JMP

// VM: label END
(FibonacciSeries$END)


// --- END INFINITE LOOP ---
(END)
@END
0;JMP

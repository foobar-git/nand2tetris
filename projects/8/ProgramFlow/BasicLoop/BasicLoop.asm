// --- BOOTSTRAP ---
@256
D=A
@SP
M=D
// VM: 	push constant 0    
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop local 0         // sum = 0
@LCL
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
(BasicLoop$LOOP)

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

// VM: 	push local 0
@LCL
D=M
@0
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

// VM: 	pop local 0	        // sum = sum + n
@LCL
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

// VM: 	pop argument 0      // n--
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

// VM: 	if-goto LOOP        // if n > 0, goto LOOP
@SP
AM=M-1
D=M
@BasicLoop$LOOP
D;JNE

// VM: 	push local 0        // else, pushes sum to the stack's top
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1


// --- END INFINITE LOOP ---
(END)
@END
0;JMP

// --- BOOTSTRAP ---
@256
D=A
@SP
M=D
@RET_ADDR_0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(RET_ADDR_0)
// VM: function Main.fibonacci 0
(Main.fibonacci)

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

// VM: 	push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	lt                     
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE1
D;JLT
@SP
A=M-1
M=0
@CONT1
0;JMP
(TRUE1)
@SP
A=M-1
M=-1
(CONT1)

// VM: 	if-goto N_LT_2        
@SP
AM=M-1
D=M
@Main$N_LT_2
D;JNE

// VM: 	goto N_GE_2
@Main$N_GE_2
0;JMP

// VM: label N_LT_2               // if n < 2 returns n
(Main$N_LT_2)

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

// VM: 	return
@LCL
D=M
@R14
M=D
@5
A=D-A
D=M
@R15
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R14
AM=M-1
D=M
@THAT
M=D
@R14
AM=M-1
D=M
@THIS
M=D
@R14
AM=M-1
D=M
@ARG
M=D
@R14
AM=M-1
D=M
@LCL
M=D
@R15
A=M
0;JMP

// VM: label N_GE_2               // if n >= 2 returns fib(n - 2) + fib(n - 1)
(Main$N_GE_2)

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

// VM: 	push constant 2
@2
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

// VM: 	call Main.fibonacci 1  // computes fib(n - 2)
@RET_ADDR_2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RET_ADDR_2)

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

// VM: 	call Main.fibonacci 1  // computes fib(n - 1)
@RET_ADDR_3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RET_ADDR_3)

// VM: 	add                    // returns fib(n - 1) + fib(n - 2)
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: 	return
@LCL
D=M
@R14
M=D
@5
A=D-A
D=M
@R15
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R14
AM=M-1
D=M
@THAT
M=D
@R14
AM=M-1
D=M
@THIS
M=D
@R14
AM=M-1
D=M
@ARG
M=D
@R14
AM=M-1
D=M
@LCL
M=D
@R15
A=M
0;JMP

// VM: function Sys.init 0
(Sys.init)

// VM: 	push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	call Main.fibonacci 1
@RET_ADDR_4
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RET_ADDR_4)

// VM: label END  
(Sys$END)

// VM: 	goto END  // loops infinitely
@Sys$END
0;JMP


// --- END INFINITE LOOP ---
(END)
@END
0;JMP

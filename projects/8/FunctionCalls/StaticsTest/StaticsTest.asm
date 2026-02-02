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
// VM: function Class1.set 0
(Class1.set)

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

// VM: 	pop static 0
@SP
AM=M-1
D=M
@Class1.0
M=D

// VM: 	push argument 1
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

// VM: 	pop static 1
@SP
AM=M-1
D=M
@Class1.1
M=D

// VM: 	push constant 0
@0
D=A
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

// VM: function Class1.get 0
(Class1.get)

// VM: 	push static 0
@Class1.0
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push static 1
@Class1.1
D=M
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

// VM: function Class2.set 0
(Class2.set)

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

// VM: 	pop static 0
@SP
AM=M-1
D=M
@Class2.0
M=D

// VM: 	push argument 1
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

// VM: 	pop static 1
@SP
AM=M-1
D=M
@Class2.1
M=D

// VM: 	push constant 0
@0
D=A
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

// VM: function Class2.get 0
(Class2.get)

// VM: 	push static 0
@Class2.0
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push static 1
@Class2.1
D=M
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

// VM: 	push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	call Class1.set 2
@RET_ADDR_1
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
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.set
0;JMP
(RET_ADDR_1)

// VM: 	pop temp 0 // dumps the return value
@SP
AM=M-1
D=M
@5
M=D

// VM: 	push constant 23
@23
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 15
@15
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	call Class2.set 2
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
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.set
0;JMP
(RET_ADDR_2)

// VM: 	pop temp 0 // dumps the return value
@SP
AM=M-1
D=M
@5
M=D

// VM: 	call Class1.get 0
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
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.get
0;JMP
(RET_ADDR_3)

// VM: 	call Class2.get 0
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
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.get
0;JMP
(RET_ADDR_4)

// VM: label END
(Sys$END)

// VM: 	goto END
@Sys$END
0;JMP


// --- END INFINITE LOOP ---
(END)
@END
0;JMP

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
// VM: function Sys.init 0
(Sys.init)

// VM: 	push constant 4000	// tests that THIS and THAT are handled correctly
@4000
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 0
@SP
AM=M-1
D=M
@3
M=D

// VM: 	push constant 5000
@5000
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 1
@SP
AM=M-1
D=M
@4
M=D

// VM: 	call Sys.main 0
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
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(RET_ADDR_1)

// VM: 	pop temp 1
@SP
AM=M-1
D=M
@6
M=D

// VM: 	label LOOP
(Sys$LOOP)

// VM: 	goto LOOP
@Sys$LOOP
0;JMP

// VM: function Sys.main 5
(Sys.main)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push constant 4001
@4001
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 0
@SP
AM=M-1
D=M
@3
M=D

// VM: 	push constant 5001
@5001
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 1
@SP
AM=M-1
D=M
@4
M=D

// VM: 	push constant 200
@200
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop local 1
@LCL
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

// VM: 	push constant 40
@40
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop local 2
@LCL
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

// VM: 	push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop local 3
@LCL
D=M
@3
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: 	push constant 123
@123
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	call Sys.add12 1
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
@Sys.add12
0;JMP
(RET_ADDR_2)

// VM: 	pop temp 0
@SP
AM=M-1
D=M
@5
M=D

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

// VM: 	push local 1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: 	push local 4
@LCL
D=M
@4
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

// VM: 	add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: 	add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: 	add
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

// VM: function Sys.add12 0
(Sys.add12)

// VM: 	push constant 4002
@4002
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: 	pop pointer 0
@SP
AM=M-1
D=M
@3
M=D

// VM: 	push constant 5002
@5002
D=A
@SP
A=M
M=D
@SP
M=M+1

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

// VM: 	push constant 12
@12
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


// --- END INFINITE LOOP ---
(END)
@END
0;JMP

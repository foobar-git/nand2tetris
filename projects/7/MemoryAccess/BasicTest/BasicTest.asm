// --- BOOTSTRAP ---
@256
D=A
@SP
M=D

// VM: push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop local 0
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

// VM: push constant 21
@21
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 22
@22
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop argument 2
@ARG
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

// VM: pop argument 1
@ARG
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

// VM: push constant 36
@36
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop this 6
@THIS
D=M
@6
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: push constant 42
@42
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 45
@45
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop that 5
@THAT
D=M
@5
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// VM: pop that 2
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

// VM: push constant 510
@510
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop temp 6
@SP
AM=M-1
D=M
@11
M=D

// VM: push local 0
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

// VM: push that 5
@THAT
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: push argument 1
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

// VM: sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

// VM: push this 6
@THIS
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: push this 6
@THIS
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// VM: sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

// VM: push temp 6
@11
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: add
@SP
AM=M-1
D=M
A=A-1
M=D+M

// --- END INFINITE LOOP ---
(END)
@END
0;JMP

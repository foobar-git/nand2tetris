// --- BOOTSTRAP ---
@256
D=A
@SP
M=D

// VM: push constant 3030
@3030
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop pointer 0
@SP
AM=M-1
D=M
@3
M=D

// VM: push constant 3040
@3040
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop pointer 1
@SP
AM=M-1
D=M
@4
M=D

// VM: push constant 32
@32
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop this 2
@THIS
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

// VM: push constant 46
@46
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop that 6
@THAT
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

// VM: push pointer 0
@3
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: push pointer 1
@4
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

// VM: push this 2
@THIS
D=M
@2
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

// VM: push that 6
@THAT
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

// --- END INFINITE LOOP ---
(END)
@END
0;JMP

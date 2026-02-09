// --- BOOTSTRAP ---
@256
D=A
@SP
M=D

// VM: push constant 111
@111
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 333
@333
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 888
@888
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: pop static 8
@SP
AM=M-1
D=M
@StaticTest.8
M=D

// VM: pop static 3
@SP
AM=M-1
D=M
@StaticTest.3
M=D

// VM: pop static 1
@SP
AM=M-1
D=M
@StaticTest.1
M=D

// VM: push static 3
@StaticTest.3
D=M
@SP
A=M
M=D
@SP
M=M+1

// VM: push static 1
@StaticTest.1
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

// VM: push static 8
@StaticTest.8
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

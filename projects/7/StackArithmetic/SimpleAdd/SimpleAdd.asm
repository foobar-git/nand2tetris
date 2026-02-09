// --- BOOTSTRAP ---
@256
D=A
@SP
M=D

// VM: push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 8
@8
D=A
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

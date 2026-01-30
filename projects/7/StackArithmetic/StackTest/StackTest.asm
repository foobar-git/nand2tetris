// --- BOOTSTRAP ---
@256
D=A
@SP
M=D

// VM: push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE0
D;JEQ
@SP
A=M-1
M=0
@CONT0
0;JMP
(TRUE0)
@SP
A=M-1
M=-1
(CONT0)

// VM: push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE1
D;JEQ
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

// VM: push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE2
D;JEQ
@SP
A=M-1
M=0
@CONT2
0;JMP
(TRUE2)
@SP
A=M-1
M=-1
(CONT2)

// VM: push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE3
D;JLT
@SP
A=M-1
M=0
@CONT3
0;JMP
(TRUE3)
@SP
A=M-1
M=-1
(CONT3)

// VM: push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE4
D;JLT
@SP
A=M-1
M=0
@CONT4
0;JMP
(TRUE4)
@SP
A=M-1
M=-1
(CONT4)

// VM: push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE5
D;JLT
@SP
A=M-1
M=0
@CONT5
0;JMP
(TRUE5)
@SP
A=M-1
M=-1
(CONT5)

// VM: push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE6
D;JGT
@SP
A=M-1
M=0
@CONT6
0;JMP
(TRUE6)
@SP
A=M-1
M=-1
(CONT6)

// VM: push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE7
D;JGT
@SP
A=M-1
M=0
@CONT7
0;JMP
(TRUE7)
@SP
A=M-1
M=-1
(CONT7)

// VM: push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE8
D;JGT
@SP
A=M-1
M=0
@CONT8
0;JMP
(TRUE8)
@SP
A=M-1
M=-1
(CONT8)

// VM: push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: push constant 53
@53
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

// VM: push constant 112
@112
D=A
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

// VM: neg
@SP
A=M-1
M=-M

// VM: and
@SP
AM=M-1
D=M
A=A-1
M=D&M

// VM: push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1

// VM: or
@SP
AM=M-1
D=M
A=A-1
M=D|M

// VM: not
@SP
A=M-1
M=!M

// --- END INFINITE LOOP ---
(END)
@END
0;JMP

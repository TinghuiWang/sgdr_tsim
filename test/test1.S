; Test File 1

loop:	L.D		F2, 0(R1)
		L.D		F0, 4(R1)
		MUL.D	F4, F0, F2
		L.D		F8, 8(R1)
		ADD.D	F6, F8, F4
		S.D		F6, 8(R1)
		SUB.D	F10, F8, F4
		S.D		F10, 4(R1)
		ADDI	R1, R1, #4
		SUBI	R3, R3, #1
		BNEZ	R3, loop
 

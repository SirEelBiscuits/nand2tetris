// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

(LOOP)
	@24576
	D=M
	@BLACK
	D;JNE
(WHITE)
	@0
	M=0
	@DRAW
	A;JMP
(BLACK)
	@0
	M=-1
(DRAW)
	@16384
	D=A
	@1
	M=D    //R1 has pixel address
(DRAWLOOP)
	@0
	D=M    //D has pixel colour
	@1     //M has pixel address
	A=M
	M=D    //pixel set
	D=A+1
	@1
	M=D
	@24576
	D=D-A
	@DRAWLOOP
	D;JLT
	@LOOP
	A;JMP
	

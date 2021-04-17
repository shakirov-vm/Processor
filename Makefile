compile:
	g++ Assembler.cpp Processor.cpp Disassembler.cpp main.cpp -o main
run:
	./main Input.txt Output.dat Dis.txt

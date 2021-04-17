compile:
	g++ Assembler.cpp AssemblerFunc.cpp AssemblerStk.cpp Processor.cpp Disassembler.cpp main.cpp -o main
listing:
	g++ Assembler.cpp AssemblerFunc.cpp AssemblerStk.cpp Processor.cpp Disassembler.cpp main.cpp -E
run:
	./main Input.txt Output.dat Dis.txt
valgrind:
	valgrind ./main Input.txt Output.dat Dis.txt
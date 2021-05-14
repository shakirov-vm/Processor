compile:
	g++ Assembler.cpp AssemblerFunc.cpp AssemblerStk.cpp Processor.cpp Disassembler.cpp main.cpp -o main
listing:
	g++ Assembler.cpp AssemblerFunc.cpp AssemblerStk.cpp Processor.cpp Disassembler.cpp main.cpp -c
run:
	./main Input.txt Output.dat Dis.txt
valgrind:
	valgrind ./main Input.txt Output.dat Dis.txt
fact:
	./main factorial.txt Output.dat Dis.txt
fib:
	./main fibbonachi.txt Output.dat Dis.txt
quad:
	./main quadro.txt Output.dat Dis.txt
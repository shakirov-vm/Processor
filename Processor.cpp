#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#ifndef ENUM_H
#define ENUM_H
#include "Enum.h"
#endif

#ifndef STACK_CONST_H    
#define STACK_CONST_H  
#include "StackConst.h"
#endif 

#if 1
#define TYPE int
#include "Stack.h"
#undef TYPE
#endif                                                        

#if 1
#define TYPE double
#include "Stack.h"
#undef TYPE
#endif

#ifndef PROCESSOR_H    
#define PROCESSOR_H
#include "Processor.h"
#endif 

#define rax registr[0]
#define rbx registr[1]
#define rcx registr[2]
#define rdx registr[3]
#define rsi registr[4]
#define rpi registr[5]

#define mode 1

Proc::Proc(size_t size_of_input)
{
	IP = 0;
	registr = (double*)calloc(NUM_OF_REG, sizeof(double));
	commands = (double*)calloc(size_of_input, sizeof(double));
	RAM  = (double*)calloc(1024, sizeof(double));
	VRAM = (char*)calloc(1024, sizeof(char));
}
Proc::~Proc()
{
	free(registr);
	free(commands);
	free(RAM);
	free(VRAM);
}

void Processor(char* input)
{
	size_t size_of_input = KnowSizeInput(input);

	class Proc CPU(size_of_input);
	
	int read_out = CPU.CommandFiller(input, size_of_input);

	CPU.stk.DUMP();
	
	CPU.CPUHandle();
}

void Proc::CPUHandle()
{
	for (int i = 0; ; i++)
	{
		switch ((int)commands[IP])
		{
			PROC_OTHER(PUSH_RAM)
			PROC_OTHER(PUSH_R)
			PROC_OTHER(POP_R)
			PROC_OTHER(PUSH)
			PROC_OTHER(OUT)
			PROC_OTHER(CALL)
			PROC_OTHER(RET)
			PROC_OTHER(JMP)		
			PROC_ARIFM(ADD, +)    
			PROC_ARIFM(MUL, *)
			PROC_ARIFM(DIV, /)
			PROC_ARIFM(SUB, -)
			PROC_JUMP(JB, <)
			PROC_JUMP(JBE, <= )
			PROC_JUMP(JA, > )
			PROC_JUMP(JAE, >= )
			PROC_JUMP(JE, == )
			PROC_JUMP(JNE, != )
			case CMD_END:
			{
				this->CPU_END();
				return;
			}
		}
	}
}

void Proc::CPU_PUSH_RAM()
{
	size_t adress = 0;
	if(((int)commands[IP + 1]) != 110)
	{
		adress = registr[(int)commands[IP + 1] - CMD_RAX] + (int)commands[IP + 2];
	}
	else
	{
		adress = (int)commands[IP + 2];
	}
	IP = IP + 3;

	if ((adress >= 0) && (adress <= 1023))
	{
		RAM[adress] = stk.Pop();
		printf("push from stack to RAM on adress %ld\n", adress);
		this->PrintRAM();
	}
	else if((adress >= 1024) && (adress <= 2047))
	{
		VRAM[adress - 1024] = (char)stk.Pop();
		printf("push from stack to VRAM on adress %ld\n", adress);
		this->PrintVRAM();
	}
	else 
	{
		printf("This adress is invalid - %ld\n", adress);
	}
}
void Proc::CPU_PUSH_R()
{
	stk.Push(registr[(int)commands[IP + 1] - CMD_RAX]);

	if(mode == 1) printf("push out of r%cx number <%.0lf>\n", (int)(commands[IP + 1] - CMD_RAX + 97), registr[(int)commands[IP + 1] - CMD_RAX]);

	IP = IP + 2;

	stk.DUMP();
	reg_ret.DUMP();

}
void Proc::CPU_POP_R()
{
	registr[(int)commands[IP + 1] - CMD_RAX] = stk.Pop();

	if(mode == 1) printf("pop in r%cx number <%.0lf>\n", (int)(commands[IP + 1] - CMD_RAX + 97), registr[(int)commands[IP + 1] - CMD_RAX]);

	IP = IP + 2;
}
void Proc::CPU_PUSH()
{
	stk.Push(commands[IP + 1]);

	if(mode == 1) printf("push %.0lf\n", commands[IP + 1]);

	IP = IP + 2;
}
void Proc::CPU_OUT()
{
	IP++;

	rax = stk.Pop();

	printf("out %.0lf\n", rax);   

}
void Proc::CPU_CALL()
{
	reg_ret.Push(IP); 

	IP = commands[IP + 1];

	if(mode == 1) printf("CALL ON %d\n",  IP);

	reg_ret.DUMP();
}
void Proc::CPU_RET()
{
	IP = reg_ret.Pop();
	IP = IP + 2;

	if(mode == 1) printf("RET ON %d\n", IP);

	reg_ret.DUMP();
}
void Proc::CPU_JMP()
{
	IP = commands[IP + 1];
}
void Proc::CPU_END()
{
	//this->PrintRAM();
	stk.DUMP();
	printf("end\n");
}




int Proc::CommandFiller(char* input, int size_of_input)
{
	FILE* potok = fopen(input, "rb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Processor: Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	int read_out = fread(commands, sizeof(double), size_of_input, potok);

	printf("\n");
	
	fclose(potok);

	return read_out;
}
void Proc::PrintRAM()
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%0.lf ", RAM[i]);
	}
	printf("\n");
}
void Proc::PrintVRAM()
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%c ", VRAM[i]);
	}
	printf("\n");
}

#include <ctype.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "Enum.h"

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

int NUM_OF_REG = 6;

#include "Processor.h"

#define rax CPU.registr[0]
#define rbx CPU.registr[1]
#define rcx CPU.registr[2]
#define rdx CPU.registr[3]
#define rsi CPU.registr[4]
#define rpi CPU.registr[5]

unsigned long KnowSizeInput(char* input);
int CommandFiller(struct Proc CPU, char* input, int size_of_input);

struct Proc
{
	double* commands;
	int IP;
	double* registr;
	Stack_double stk;
	Stack_int reg_ret;
	double* RAM;
	char* VRAM;
};

// int yes = 1;

void CPU(char* input)
{
	struct Proc CPU;

	unsigned long size_of_input = KnowSizeInput(input);

	CPU.IP = 0;
	CPU.registr = (double*)calloc(NUM_OF_REG, sizeof(double));
	CPU.commands = (double*)calloc(size_of_input, sizeof(double));
	CPU.RAM  = (double*)calloc(1024, sizeof(double));
	CPU.VRAM = (char*)calloc(1024, sizeof(char));

	int read_out = CommandFiller(CPU, input, size_of_input);

	CPU.stk.DUMP();
	
	for (int i = 0; ; i++)
	{
		switch ((int)CPU.commands[CPU.IP])
		{
			case CMD_PUSH_RAM:
			{
				size_t adress = 0;
				if(((int)CPU.commands[CPU.IP + 1]) != 110)
				{
					adress = CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX] + (int)CPU.commands[CPU.IP + 2];
				}
				else
				{
					adress = (int)CPU.commands[CPU.IP + 2];
				}
				CPU.IP = CPU.IP + 3;

				if ((adress >= 0) && (adress <= 1023))
				{
					CPU.RAM[adress] = CPU.stk.Pop();
					printf("push from stack to RAM on adress %d\n", adress);
					//PrintRAM(&CPU);
				}
				else if((adress >= 1024) && (adress <= 2047))
				{
					CPU.VRAM[adress - 1024] = (char)CPU.stk.Pop();
					printf("push from stack to VRAM on adress %d\n", adress);
					//PrintVRAM(&CPU);
				}
				else 
				{
					printf("This adress is invalid - %d\n", adress);
				}
				continue;
			}
			case CMD_PUSH_R:
			{
				CPU.stk.Push(CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#if 1
				printf("push out of r%cx number <%.0lf>\n", (int)(CPU.commands[CPU.IP + 1] - CMD_RAX + 97), CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#endif
				CPU.IP = CPU.IP + 2;

				CPU.stk.DUMP();
				CPU.reg_ret.DUMP();

				break;
			}
			case CMD_POP_R:
			{
				CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX] = CPU.stk.Pop();
#if 1
				printf("pop in r%cx number <%.0lf>\n", (int)(CPU.commands[CPU.IP + 1] - CMD_RAX + 97), CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#endif
				CPU.IP = CPU.IP + 2;

				break;
			}
			case CMD_PUSH: 
			{
				CPU.stk.Push(CPU.commands[CPU.IP + 1]);
#if 1
				printf("push %.0lf\n", CPU.commands[CPU.IP + 1]);
#endif
				CPU.IP = CPU.IP + 2;
				
				break;
			}
			PROC_ARIFM(ADD, +)    
			PROC_ARIFM(MUL, *)
			PROC_ARIFM(DIV, /)
			PROC_ARIFM(SUB, -)
			case CMD_OUT:
			{
				CPU.IP++;

				rax = CPU.stk.Pop();

				printf("out %.0lf\n", rax);   

				break;
			}
			case CMD_CALL:
			{
				CPU.reg_ret.Push(CPU.IP); 

				CPU.IP = CPU.commands[CPU.IP + 1];
#if 0
				printf("CALL ON %d\n",  CPU.IP);
#endif
				CPU.reg_ret.DUMP();

				break;
			}
			case CMD_RET:
			{
				CPU.IP = CPU.reg_ret.Pop();
				CPU.IP = CPU.IP + 2;
#if 0
				printf("RET ON %d\n", CPU.IP);
#endif
				CPU.reg_ret.DUMP();
				break;
			}
			case CMD_JMP:
			{
				CPU.IP = CPU.commands[CPU.IP + 1];

				break;
			}
			PROC_JUMP(JB, <)
			PROC_JUMP(JBE, <= )
			PROC_JUMP(JA, > )
			PROC_JUMP(JAE, >= )
			PROC_JUMP(JE, == )
			PROC_JUMP(JNE, != )
			case CMD_END:
			{
				PrintRAM(&CPU);
				CPU.stk.DUMP();
				printf("end\n");
				return;
			}
		}
	}
}

unsigned long KnowSizeInput(char* input)
{
	struct stat buff;
	stat(input, &buff);
	return buff.st_size;
}

int CommandFiller(struct Proc CPU, char* input, int size_of_input)
{
	FILE* potok = fopen(input, "rb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Processor: Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	int read_out = fread(CPU.commands, sizeof(double), size_of_input, potok);

	printf("\n");
	
	fclose(potok);

	return read_out;
}
void PrintRAM(struct Proc* CPU)
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%0.lf ", CPU->RAM[i]);
	}
	printf("\n");
}
void PrintVRAM(struct Proc* CPU)
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%0.lf ", CPU->VRAM[i]);
	}
	printf("\n");
}

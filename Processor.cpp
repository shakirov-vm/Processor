#include <ctype.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "Enum.h"

#ifndef  _STACK_H
#define _STACK_H

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
};

// int yes = 1;

void CPU(char* input)
{
	struct Proc CPU;

	CPU.IP = 0;
	CPU.registr = (double*)calloc(NUM_OF_REG, sizeof(double));

	unsigned long size_of_input = KnowSizeInput(input);

	CPU.commands = (double*)calloc(size_of_input, sizeof(double));


	int read_out = CommandFiller(CPU, input, size_of_input);

	Stack_double stk;
	Stack_double reg_ret;
	stk.DUMP();
	
	for (int i = 0; /*i < size_of_input*/; i++)
	{
		switch ((int)CPU.commands[CPU.IP])
		{
			case CMD_PUSH_R:
			{
				stk.Push(CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#if 0
				printf("push out of r%cx number <%.0lf>\n", (int)(CPU.commands[CPU.IP + 1] - CMD_RAX + 97), CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#endif
				CPU.IP = CPU.IP + 2;

				stk.DUMP();
				reg_ret.DUMP();

				break;
			}
			case CMD_POP_R:
			{
				CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX] = stk.Pop();
#if 0
				printf("pop in r%cx number <%.0lf>\n", (int)(CPU.commands[CPU.IP + 1] - CMD_RAX + 97), CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
#endif
				CPU.IP = CPU.IP + 2;

				break;
			}
			case CMD_PUSH: 
			{
				stk.Push(CPU.commands[CPU.IP + 1]);
#if 0
				printf("push %.0lf\n", CPU.commands[CPU.IP + 1]);
#endif
				CPU.IP = CPU.IP + 2;
				
				break;
			}
			PROC_ARIFM(ADD, +)      // Вроде работает
			PROC_ARIFM(MUL, *)
			PROC_ARIFM(DIV, /)
			PROC_ARIFM(SUB, -)
			case CMD_OUT:
			{
				CPU.IP++;

				rax = stk.Pop();

				printf("out %.0lf\n", rax);

				//stk.Push(rax);                                    КАК ЭТО СТОИТ РЕАЛИЗОВАТЬ??          

				//stk.DUMP();

				break;
			}
			case CMD_CALL:
			{
				reg_ret.Push(CPU.IP); //                              ?????????????

				CPU.IP = CPU.commands[CPU.IP + 1];

				//stk.DUMP();
#if yes
				printf("CALL ON %d\n",  CPU.IP);
#endif
				reg_ret.DUMP();

				break;
			}
			case CMD_RET:
			{
				CPU.IP = reg_ret.Pop();
				CPU.IP = CPU.IP + 2;
#if yes
				printf("RET ON %d\n", CPU.IP);
#endif
				reg_ret.DUMP();
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
				stk.DUMP();
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

	//printf(">>> %d - readed\n\n", read_out);

	/*for (int i = 0; i <= read_out; i++)
	{
		printf("%d - %.0lf\n", i, CPU.commands[i]);
	}*/
	printf("\n");
	
	fclose(potok);

	return read_out;
}
/*
void print_reg(struct Proc* CPU)
{
	printf("-------------------------\n| rax | rbx | rcx | rdx |\n-------------------------\n|%0.lf|%0.lf|%0.lf|%0.lf|\n-------------------------\n", CPU->register[0], CPU->register[1], CPU->register[2], CPU->register[3]);
}*/
 
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

#define rax CPU.registr[0]
#define rbx CPU.registr[1]
#define rcx CPU.registr[2]
#define rdx CPU.registr[3]

unsigned long KnowSizeInput(char* input);
int CommandFiller(struct Proc CPU, char* input, int size_of_input);

struct Proc
{
	double* commands;
	int IP;
	double* registr;
};

void CPU(char* input)
{
	struct Proc CPU;

	CPU.IP = 0;
	CPU.registr = (double*)calloc(4, sizeof(double));

	unsigned long size_of_input = KnowSizeInput(input);

	CPU.commands = (double*)calloc(size_of_input, sizeof(double));


	int read_out = CommandFiller(CPU, input, size_of_input);

	Stack_double stk;
	stk.DUMP();
	
	for (int i = 0; /*i < size_of_input*/; i++)
	{
		switch ((int)CPU.commands[CPU.IP])
		{
			case CMD_PUSH_R:
			{
				stk.Push(CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
				//printf("push in registr %.0lf number %.0lf\n", CPU.commands[CPU.IP + 1], CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
				CPU.IP = CPU.IP + 2;

				break;
			}
			case CMD_POP_R:
			{
				CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX] = stk.Pop();
				//printf("pop in registr %.0lf number %.0ld\n", CPU.commands[CPU.IP + 1], CPU.registr[(int)CPU.commands[CPU.IP + 1] - CMD_RAX]);
				CPU.IP = CPU.IP + 2;

				break;
			}
			case CMD_PUSH: 
			{
				stk.Push(CPU.commands[CPU.IP + 1]);
				//printf("push %.0lf\n", CPU.commands[CPU.IP + 1]);
				CPU.IP = CPU.IP + 2;
				
				break;
			}
			case CMD_ADD:
			{
				CPU.IP++;

				rax = stk.Pop();
				rbx = stk.Pop();

				//printf("add %.0lf %.0lf\n", rax, rbx);

				rax = rax + rbx;

				stk.Push(rax);

				break;
			}
			case CMD_OUT:
			{
				CPU.IP++;

				rax = stk.Pop();

				printf("out %.0lf\n", rax);

				stk.Push(rax);

				stk.DUMP();

				break;
			}
			case CMD_MUL:
			{
				CPU.IP++;

				rax = stk.Pop();
				rbx = stk.Pop();

				//printf("mul %.0lf %.0lf\n", rax, rbx);

				rax = rax * rbx;

				stk.Push(rax);

				break;
			}
			case CMD_SUB:
			{
				CPU.IP++;

				rax = stk.Pop();
				rbx = stk.Pop();

				//printf("sub %.0lf %.0lf\n", rax, rbx);

				rax = rax - rbx;

				stk.Push(rax);

				break;
			}
			case CMD_DIV:
			{
				CPU.IP++;

				rax = stk.Pop();
				rbx = stk.Pop();

				//printf("div %.0lf %.0lf\n", rax, rbx);

				rax = rax / rbx;

				stk.Push(rax);

				break;
			}
			case CMD_JMP:
			{
				//printf("JUMP\n\n");

				CPU.IP = CPU.commands[CPU.IP + 1];

				break;
			}
			case CMD_END:
			{
				stk.DUMP();

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

	int read_out = fread(CPU.commands, sizeof(double), size_of_input, potok);

	printf(">>> %d - readed\n\n", read_out);

	for (int i = 0; i <= read_out; i++)
	{
		printf("%.0lf ", CPU.commands[i]);
	}
	printf("\n");

	fclose(potok);

	return read_out;
}
 
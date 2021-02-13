
#include "Enum.h"

#ifndef  _STACK_H
#define _STACK_H

#define TYPE int
#include "Stack.h"
#undef TYPE

#endif

void CPU(char* input)
{
	struct stat buff;
	stat(input, &buff);

	unsigned long size_of_input = buff.st_size;

	size_of_input = size_of_input;

	double* commands = (double*)calloc(size_of_input, sizeof(double));

	FILE* potok = fopen(input, "rb");

	int true_fread = fread(commands, sizeof(double), size_of_input, potok);

	printf(">>>>> %d\n\n", true_fread);

	for (int i = 0; i < 9; i++)
	{
		printf("%lf ", commands[i]);
	}
	printf("\n");

	fclose(potok);

	int IP = 0;

	Stack_int stk;

	for (int i = 0; i < size_of_input; i++)
	{
		switch ((int)commands[IP])
		{
			case CMD_PUSH: 
			{
				stk.Push(commands[IP + 1]);
				IP = IP + 2;
				
				break;
			}
			case CMD_ADD:
			{
				IP++;

				int a = stk.Pop();
				int b = stk.Pop();

				printf(">>>>>>>>>> %d %d\n", a, b);

				int c = a + b;

				stk.Push(c);

				break;
			}
			case CMD_OUT:
			{
				IP++;

				int a = stk.Pop();

				printf("%d ", a);

				stk.Push(a);

				break;
			}
		}
	}
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#include "Enum.h"

int MAX_COM_SIZE = 20;

void Dissambler(char* input, char* output)
{
	struct stat buff;
	stat(input, &buff);

	unsigned long size_of_input = buff.st_size;
	                                         
	double* disassembliruemoe = (double*)calloc(size_of_input * sizeof(double), 1);

	FILE* potok = fopen(input, "rb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Disassembler: Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	fread(disassembliruemoe, sizeof(double), size_of_input, potok);  

	fclose(potok);

	potok = fopen(output, "w");
	if (errno)
	{
		char answer[100];

		sprintf(answer, "Disassembler: Problem file: %s\n", output);

		perror(answer);
		exit(1);
	}
	                   //  ???
	for (int i = 0; i < size_of_input; i++)
	{
		switch ((int)disassembliruemoe[i])
		{
			case CMD_PUSH:
			{
				fprintf(potok, "push %lf\n", disassembliruemoe[i + 1]);
				i++;
				break;
			}
			case CMD_PUSH_R:
			{
				switch ((int)disassembliruemoe[i + 1])
				{
					case CMD_RAX:
					{
						fprintf(potok, "push rax\n");
						break;
					}
					case CMD_RBX:
					{
						fprintf(potok, "push rbx\n");
						break;
					}
					case CMD_RCX:
					{
						fprintf(potok, "push rcx\n");
						break;
					}
					case CMD_RDX:
					{
						fprintf(potok, "push rdx\n");
						break;
					}
					case CMD_RSI:
					{
						fprintf(potok, "push rsi\n");
						break;
					}
					case CMD_RPI:
					{
						fprintf(potok, "push rpi\n");
						break;
					}
				}
				i++;
				break;
			}
			case CMD_PUSH_RAM:
			{
				if ((int)disassembliruemoe[i + 1] == CMD_NOT_REG)
				{
					fprintf(potok, "push [%d]\n", (int)disassembliruemoe[i + 2]);
				}
				else
				{
					fprintf(potok, "push [r%cx + %d]\n", (int)disassembliruemoe[i + 1] - CMD_RAX + 97, (int)disassembliruemoe[i + 2]);
				}
			}
			case CMD_POP_R:
			{
				switch ((int)disassembliruemoe[i + 1])
				{
					case CMD_RAX:
					{
						fprintf(potok, "pop rax\n");
						break;
					}
					case CMD_RBX:
					{
						fprintf(potok, "pop rbx\n");
						break;
					}
					case CMD_RCX:
					{
						fprintf(potok, "pop rcx\n");
						break;
					}
					case CMD_RDX:
					{
						fprintf(potok, "pop rdx\n");
						break;
					}
					case CMD_RSI:
					{
						fprintf(potok, "pop rsi\n");
						break;
					}
					case CMD_RPI:
					{
						fprintf(potok, "pop rpi\n");
						break;
					}
				}
				i++;
				break;
			}
			case CMD_POP:
			{
				fprintf(potok, "pop\n");
				break;
			}
			case CMD_ADD:
			{
				fprintf(potok, "add\n");
				break;
			}
			case CMD_SUB:
			{
				fprintf(potok, "sub\n");
				break;
			}
			case CMD_MUL:
			{
				fprintf(potok, "mul\n");
				break;
			}
			case CMD_DIV:
			{
				fprintf(potok, "div\n");
				break;
			}
			case CMD_OUT:
			{
				fprintf(potok, "out\n");
				break;
			}
			case CMD_END:
			{
				fprintf(potok, "end\n");
				break;
			}
			case CMD_RET:
			{
				fprintf(potok, "ret\n");
				break;
			}
			case CMD_JMP:
			{
				fprintf(potok, "jmp %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JB:
			{
				fprintf(potok, "jb %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JBE:
			{
				fprintf(potok, "jbe %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JA:
			{
				fprintf(potok, "ja %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JAE:
			{
				fprintf(potok, "jae %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JNE:
			{
				fprintf(potok, "jne %lf\n", disassembliruemoe[i + 1]);
				break;
			}
			case CMD_JE:
			{
				fprintf(potok, "je %lf\n", disassembliruemoe[i + 1]);
				break;
			}
		}
	}
	fclose(potok);
	free(disassembliruemoe);
}
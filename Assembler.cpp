#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#include "Enum.h"

// ��� �������� ������� ����� ��� ��������� ������ ������ � ��������� rbx � rcx
// �������� ������ ����������� �����, ����� ������� ��������� (�.�. je ���������, ���� rbx = rcx)

int strcmp_my(char* line, char* tag);

void Assembler(char* input, char* output)
{
	struct stat buff;
	stat(input, &buff);

	unsigned long size_of_input = buff.st_size;

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));

	FILE* potok = fopen(input, "r+");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Assembler: Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	fread(assembliruemoe, sizeof(char), size_of_input, potok);

	fclose(potok);

	double* command = (double*)calloc(size_of_input, sizeof(int));                            ///////

	char** lines = (char**)calloc(size_of_input, sizeof(char*));

	lines[0] = assembliruemoe;

	size_t count_lines = 1;
																						//�������� � ���������� ������� (����� �����)
	for (int i = 0; i < size_of_input; i++) 
	{
		if (assembliruemoe[i] == ' ' || assembliruemoe[i] == '\n')	
		{
			lines[count_lines] = assembliruemoe + i + 1;
			count_lines++;
			assembliruemoe[i] = '\0';
		}
	}

	size_t count_command = 0;															//����� ��?
	char* ptr;

	char** tags = (char**)calloc(count_lines, sizeof(char*));
	size_t count_tags = 0;
	int* place = (int*)calloc(count_lines, sizeof(int));



	for (int i = 0; i < count_lines; i++) // continue � ����� ������� if
	{
		if (strcmp(lines[i], "push") == 0) {count_command = count_command + 2; i++;}
		if (strcmp(lines[i], "pop") == 0)  {count_command = count_command + 2; i++;}
		if (strcmp(lines[i], "add") == 0)	count_command++;
		if (strcmp(lines[i], "mul") == 0)	count_command++;
		if (strcmp(lines[i], "sub") == 0)	count_command++;
		if (strcmp(lines[i], "div") == 0)	count_command++;
		if (strcmp(lines[i], "out") == 0)	count_command++;
		if (strcmp(lines[i], "end") == 0)	count_command++;
		if (strcmp(lines[i], "ret") == 0)	count_command++;
		if (strcmp(lines[i], "call") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "jmp") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "jb") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "jbe") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "ja") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "jae") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "je") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (strcmp(lines[i], "jne") == 0)
		{
			count_command = count_command + 2;
			continue;
		}
		if (i != count_lines - 1)
		{
			if ((lines[i + 1][-2] == ':'))
			{
				tags[count_tags] = lines[i];
				place[count_tags] = count_command;    //                               ��� ����� �����������??
				count_tags++;
			}
		}
	}
//=========================================================================================================================================
	count_command = 0;                                                              //         COPYPASTE
//=========================================================================================================================================
	for (int i = 0; i < count_lines; i++) // continue � ����� ������� if
	{
		if (strcmp(lines[i], "push") == 0)
		{
			double argument = strtod(lines[i + 1], &ptr);
			count_command++;
			i++;

			if (*ptr == '\0')
			{
				command[count_command - 1] = CMD_PUSH;
				command[count_command] = argument;
			}

			else
			{
				command[count_command - 1] = CMD_PUSH_R;
				if (*(ptr + 1) == 'a') command[count_command] = CMD_RAX;
				if (*(ptr + 1) == 'b') command[count_command] = CMD_RBX;
				if (*(ptr + 1) == 'c') command[count_command] = CMD_RCX;
				if (*(ptr + 1) == 'd') command[count_command] = CMD_RDX;
				if (*(ptr + 1) == 's') command[count_command] = CMD_RSI;
				if (*(ptr + 1) == 'p') command[count_command] = CMD_RPI;
			}
			count_command++;
		}
		if (strcmp(lines[i], "pop") == 0)
		{
			double argument = strtod(lines[i + 1], &ptr);
			count_command++;
			i++;

			if (*ptr == '\0')
			{
				command[count_command - 1] = CMD_POP;
				command[count_command] = argument;
			}

			else
			{
				command[count_command - 1] = CMD_POP_R;
				if (*(ptr + 1) == 'a') command[count_command] = CMD_RAX;
				if (*(ptr + 1) == 'b') command[count_command] = CMD_RBX;
				if (*(ptr + 1) == 'c') command[count_command] = CMD_RCX;
				if (*(ptr + 1) == 'd') command[count_command] = CMD_RDX;
			}
			count_command++;
		}
		if (strcmp(lines[i], "add") == 0)
		{
			command[count_command] = CMD_ADD;
			count_command++;
		}
		if (strcmp(lines[i], "mul") == 0)
		{
			command[count_command] = CMD_MUL;
			count_command++;
		}
		if (strcmp(lines[i], "sub") == 0)
		{
			command[count_command] = CMD_SUB;
			count_command++;
		}
		if (strcmp(lines[i], "div") == 0)
		{
			command[count_command] = CMD_DIV;
			count_command++;
		}
		if (strcmp(lines[i], "out") == 0)
		{
			command[count_command] = CMD_OUT;
			count_command++;
		}
		if (strcmp(lines[i], "end") == 0)
		{
			command[count_command] = CMD_END;
			count_command++;
		}
		if (strcmp(lines[i], "ret") == 0)
		{
			command[count_command] = CMD_RET;
			count_command++;
		}
		if (strcmp(lines[i], "jmp") == 0)
		{
			command[count_command] = CMD_JMP;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "call") == 0)
		{
			command[count_command] = CMD_CALL;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "jb") == 0)
		{
			command[count_command] = CMD_JB;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "jbe") == 0)
		{
			command[count_command] = CMD_JBE;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "ja") == 0)
		{
			command[count_command] = CMD_JA;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "jae") == 0)
		{
			command[count_command] = CMD_JAE;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "je") == 0)
		{
			command[count_command] = CMD_JE;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
		if (strcmp(lines[i], "jne") == 0)
		{
			command[count_command] = CMD_JNE;
			count_command++;
			i++;

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; //                                                    ???
					break;
				}
			}
		}
	}


	potok = fopen(output, "wb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Assembler: Problem file: %s\n", output);

		perror(answer);
		exit(2);
	}
	printf("<<< %d %d\n", count_command, count_lines);
	for (int i = 0; i < count_command; i++)
	{
		printf("%.0lf ", command[i]);
	}
	printf("\n");

	int entered = fwrite(command, sizeof(double), count_command, potok);

	//printf(">>> %d\n", entered);

	fclose(potok);

	free(assembliruemoe);
	free(lines);
	free(command);
}

int strcmp_my(char* line, char* tag)
{
	int differ = 0;

	for (int i = 0; tag[i] != ':'; i++)
	{
		differ = line[i] - tag[i];

		if (differ != 0) return differ;
	}
	return 0;
}

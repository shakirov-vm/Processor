#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#include "Enum.h"

void FileEnter(char* input, char* output)
{
	struct stat buff;
	stat(input, &buff);

	unsigned long size_of_input = buff.st_size;

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));

	FILE* potok = fopen(input, "r+");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	fread(assembliruemoe, sizeof(char), size_of_input, potok);

	fclose(potok);

	double* command = (double*)calloc(size_of_input, sizeof(int));                            ///////

	char** lines = (char**)calloc(size_of_input, sizeof(char*));

	lines[0] = assembliruemoe;

	size_t count_lines = 1;

	for (int i = 0; i < size_of_input; i++) 
	{
		if (assembliruemoe[i] == ' ')	
		{
			lines[count_lines] = assembliruemoe + i + 1;
			count_lines++;
			assembliruemoe[i] = '\0';
		}
	}

	size_t count_command = 0;

	for (int i = 0; i < count_lines; i++)
	{
		if (strcmp(lines[i], "push") == 0)
		{
			command[count_command] = CMD_PUSH;
			count_command++;
			i++;
			double argument = atof(lines[i]);
			command[count_command] = argument;
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
		if (strcmp(lines[i], "pop") == 0)
		{
			command[count_command] = CMD_POP;
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
		
	}

	potok = fopen(output, "wb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Problem file: %s\n", output);

		perror(answer);
		exit(2);
	}

	for (int i = 0; i < 18; i++)
	{
		printf("%d ", command[i]);
	}
	printf("\n");

	int true_fwrite = fwrite(command, sizeof(double), count_command, potok);

	printf(">>>%d\n", true_fwrite);

	fclose(potok);

	free(assembliruemoe);
	free(lines);
	free(command);
}

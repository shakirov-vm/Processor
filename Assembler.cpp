#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include "Enum.h"
#include "Assembler.h"

// For conditional jumps, the numbers for comparison must be in the rbx and rcx registers
// Conditional jumps are triggered when the condition (i.e. je is triggered if rbx = rcx)
// YOU MUST PUT EXACTLY ONE SEPARATOR BETWEEN PUSH AND ARGUMENT. IF IT'S NOT SO - UNKNOWN TEAM. If the register name is spelled incorrectly
// - it will give an error with an incorrect register name. 

struct Labels
{
	char** tags;
	size_t count_tags;
	int* place;
};
struct Cmd
{
	double* command;
	size_t count_command;
};

void Assembler(char* input, char* output)    
{
	unsigned long size_of_input = KnowFileSize(input);

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));

	struct Cmd CMD;

	CMD.command = (double*)calloc(size_of_input, sizeof(int));
	CMD.count_command = 0;
	
	char** lines = (char**)calloc(size_of_input, sizeof(char*));

	FileReader(input, assembliruemoe, size_of_input);

//=================================================================		
	char** strings = (char**) calloc(size_of_input, sizeof(char*));
	size_t count_strings = 1;
	strings[0] = assembliruemoe;
	for (int i = 0; i < size_of_input; i++)
	{
		if (assembliruemoe[i] == '\n')
		{
			strings[count_strings] = assembliruemoe + i + 1;
			count_strings++;
		}
	}
//==================================================================

	lines[0] = assembliruemoe;    
	size_t count_lines = 1;

	for (int i = 0; i < size_of_input; i++) 
	{
		if (isspace(assembliruemoe[i]))	
		{
			lines[count_lines] = assembliruemoe + i + 1;                               
			count_lines++;
			assembliruemoe[i] = '\0';
		}
	}														

	struct Labels lbl;

	lbl.tags = (char**)calloc(count_lines, sizeof(char*));
	lbl.count_tags = 0;
	lbl.place = (int*)calloc(count_lines, sizeof(int));

	size_t num_errors = 0;

	for (int i = 0; i < count_lines; i++)
	{
		COMMANDS("push", 2, 1)
		COMMANDS("pop", 2, 1)
		COMMANDS("add", 1, 0)
		COMMANDS("mul", 1, 0)
		COMMANDS("sub", 1, 0)
		COMMANDS("div", 1, 0)
		COMMANDS("out", 1, 0)
		COMMANDS("end", 1, 0)
		COMMANDS("ret", 1, 0)
		COMMANDS("call", 2, 0)
		COMMANDS("jmp", 2, 0)
		COMMANDS("jb", 2, 0)
		COMMANDS("jbe", 2, 0)
		COMMANDS("ja", 2, 0)
		COMMANDS("jae", 2, 0)
		COMMANDS("je", 2, 0)
		COMMANDS("jne", 2, 0)
		if (i != count_lines - 1)
		{
			if ((lines[i + 1][-2] == ':'))
			{
				lbl.tags[lbl.count_tags] = lines[i];
				lbl.place[lbl.count_tags] = CMD.count_command;
				lbl.count_tags++;
			}
		}
	}
//=========================================================================================================================================
	CMD.count_command = 0;
	count_strings = 0;
//=========================================================================================================================================
	char* ptr;

	for (int i = 0; i < count_lines; i++) 
	{
		//printf("%d - [%s]\n", CMD.count_command, lines[i]);

		if (strcmp(lines[i], "push") == 0)
		{
			double argument = strtod(lines[i + 1], &ptr);
			CMD.count_command++;
			i++;

/* 1 */		if (*ptr == '\0')
			{
				CMD.command[CMD.count_command - 1] = CMD_PUSH;
				CMD.command[CMD.count_command] = argument;
				CMD.count_command++;
			}
/* 2 */		else if (*(ptr) == 'r')
			{
				CMD.command[CMD.count_command - 1] = CMD_PUSH_R;
					 if ((*(ptr + 1) == 'a') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RAX;
				else if ((*(ptr + 1) == 'b') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RBX;
				else if ((*(ptr + 1) == 'c') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RCX;
				else if ((*(ptr + 1) == 'd') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RDX;
				else if ((*(ptr + 1) == 's') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RSI;
				else if ((*(ptr + 1) == 'p') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RPI;
				else
				{
					printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);
					num_errors++;
				}
				CMD.count_command++;
			}
/* 3 */		else if (*(ptr) == '[') 
			{
				if (*(ptr + 1) == 'r')
				{
					CMD.command[CMD.count_command - 1] = CMD_PUSH_RAM;

						 if ((*(ptr + 2) == 'a') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RAX;
					else if ((*(ptr + 2) == 'b') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RBX;
					else if ((*(ptr + 2) == 'c') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RCX;
					else if ((*(ptr + 2) == 'd') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RDX;
					else if ((*(ptr + 2) == 's') && (*(ptr + 3) == 'i')) CMD.command[CMD.count_command] = CMD_RSI;
					else if ((*(ptr + 2) == 'p') && (*(ptr + 3) == 'i')) CMD.command[CMD.count_command] = CMD_RPI;
					else
					{
						printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);
						num_errors++;
					}

					if (*(ptr + 4) == ']')
					{
						CMD.command[CMD.count_command - 1] = CMD_PUSH_RAM;
						CMD.command[CMD.count_command + 1] = 0;
						CMD.count_command = CMD.count_command + 2;                                                   
					}
					else if (*(ptr + 5) == '+')
					{
						if (isdigit(*(ptr + 7)))
						{
							argument = strtod (ptr + 7, &ptr);

							if (*(ptr) == ']')
							{
								CMD.command[CMD.count_command - 1] = CMD_PUSH_RAM;
								CMD.command[CMD.count_command + 1] = argument;
								CMD.count_command = CMD.count_command + 2;
							}
							else 
							{
								printf("Error in [rax + num] - you forgot <<]>>. It's on line %d\n", count_strings + 1);
								num_errors++;
							}
						}
						else
						{
							printf("Error in [rax + num] - waiting number. It's on line %d\n", count_strings + 1);
							num_errors++;
						}
					}
					else
					{
						printf("Error in [rax]. It's on line %d\n", count_strings + 1);
						num_errors++;
					}
				}
				else if (isdigit(*(ptr + 1)))	
				{	
					argument = strtod(ptr + 1, &ptr);

					if (*(ptr) == ']')
					{
						CMD.command[CMD.count_command - 1] = CMD_PUSH_RAM;
						CMD.command[CMD.count_command - 0] = CMD_NOT_REG;
						CMD.command[CMD.count_command + 1] = argument;
						CMD.count_command = CMD.count_command + 2;
					}
					else
					{
						printf("Error in [num]. It's on line %d\n", count_strings + 1);
						num_errors++;
					}
				}
				else
				{
					printf("Error in []. It's on line %d\n", count_strings + 1);
					num_errors++;
				}
			}
/* 4 */		else
			{
				num_errors++;
				printf("Invalid push argument %d - %s\n", count_strings + 1, lines[i]);

				CMD.count_command--;
			}
		}
		else if (strcmp(lines[i], "pop") == 0)
		{
			double argument = strtod(lines[i + 1], &ptr);
			CMD.count_command++;
			i++;

			if (*ptr == '\0')
			{
				CMD.command[CMD.count_command - 1] = CMD_POP;
				CMD.command[CMD.count_command] = argument;
			}

			else if (*(ptr) == 'r')
			{
				CMD.command[CMD.count_command - 1] = CMD_POP_R;
					 if ((*(ptr + 1) == 'a') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RAX;
				else if ((*(ptr + 1) == 'b') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RBX;
				else if ((*(ptr + 1) == 'c') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RCX;
				else if ((*(ptr + 1) == 'd') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RDX;
				else if ((*(ptr + 1) == 's') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RSI;
				else if ((*(ptr + 1) == 'p') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RPI;
				else
				{
					printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);
					num_errors++;
				}
			}
			else if (*(ptr) == '[')  
			{
				printf("FIND\n");
			}
			else
			{
				num_errors++;
				printf("Invalid pop argument %d - %s\n", count_strings + 1, lines[i]);

				CMD.count_command--;
			}
			CMD.count_command++;
		}
		ARIFMETICAL("add", ADD)
		ARIFMETICAL("mul", MUL)
		ARIFMETICAL("sub", SUB)
		ARIFMETICAL("div", DIV)
		ARIFMETICAL("out", OUT)
		ARIFMETICAL("end", END)
		ARIFMETICAL("ret", RET)
		JUMPS("jmp", JMP)
		JUMPS("call", CALL)
		JUMPS("jb", JB)
		JUMPS("jbe", JBE)
		JUMPS("ja", JA)
		JUMPS("jae", JAE)
		JUMPS("je", JE)      // Want to define
		else if (strcmp(lines[i], "jne") == 0)																\
		{																									\
			CMD.command[CMD.count_command] = CMD_JNE;																\
			CMD.count_command++;																				\
			i++;																							\
																											\
			int find = 0;																					\
																											\
			for (int j = 0; j < lbl.count_tags; j++)															\
			{																								\
				if (strcmp_my(lines[i], lbl.tags[j]) == 0)														\
				{																							\
					CMD.command[CMD.count_command] = lbl.place[j];														\
					CMD.count_command++;																		\
					find++;																					\
																											\
					break;																					 	
				}																							\
			}																								\
			if (find == 0)																					\
			{																								\
				printf("This tag - <%s> - is unknown. Error. Line is %d\n", lines[i], count_strings + 1);	\
				num_errors++;																				\
			}																								\
		}	
		else if (strcmp(lines[i], "") == 0);        
 		else if ((lines[i + 1][-2] == ':'));
		else 
		{
			if (strcmp(strings[count_strings], ""))
			{
				num_errors++;
				printf("Unknown command on line %d - %s\n", count_strings + 1, lines[i]);
			}
		}
		if (strings[count_strings] < lines[i]) count_strings++;
	}

	if (num_errors != 0)
	{
		printf("You made %d syntax errors\n", num_errors);
		free(assembliruemoe);
		free(lines);
		free(CMD.command);
	}

	BinaryWriter(output, CMD.count_command, count_lines, CMD.command);

	assert(!num_errors);															// Return BACK

	free(assembliruemoe);
	free(lines);
	free(CMD.command);
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

void FileReader(char* input, char* assembliruemoe, int size_of_input)
{
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
}

void BinaryWriter(char* output, int count_command, int count_lines, double* command)
{
	FILE* potok = fopen(output, "wb");

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

	fclose(potok);
}

unsigned long KnowFileSize(char* input)
{
	struct stat buff;
	stat(input, &buff);

	return buff.st_size;
}


//================================================================================================
// Code to fix:

/*
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

			else if (*(ptr) == 'r')
			{
				command[count_command - 1] = CMD_PUSH_R;
					 if ((*(ptr + 1) == 'a') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RAX;
				else if ((*(ptr + 1) == 'b') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RBX;
				else if ((*(ptr + 1) == 'c') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RCX;
				else if ((*(ptr + 1) == 'd') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RDX;
				else if ((*(ptr + 1) == 's') & (*(ptr + 2) == 'i')) command[count_command] = CMD_RSI;
				else if ((*(ptr + 1) == 'p') & (*(ptr + 2) == 'i')) command[count_command] = CMD_RPI;
				else
				{
					printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);
					num_errors++;
				}
			}
			else if (*(ptr) == '[') 
			{
				double argument = strtod(lines[i + 1], &ptr);
				command[count_command + 1] = 0; // Register not specified           ????????
				command[count_command + 1] = argument;

				if (*(ptr + 1) == '+')
				{
					if (*(ptr + 3) == 'r')
					{
						command[count_command - 1] = CMD_PUSH_RAM;
						command[count_command + 1] = argument;
						if ((*(ptr + 1) == 'a') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RAX;
						else if ((*(ptr + 4) == 'b') & (*(ptr + 5) == 'x') & (*(ptr + 6) == ']')) command[count_command] = CMD_RBX;
						else if ((*(ptr + 4) == 'c') & (*(ptr + 5) == 'x') & (*(ptr + 6) == ']')) command[count_command] = CMD_RCX;
						else if ((*(ptr + 4) == 'd') & (*(ptr + 5) == 'x') & (*(ptr + 6) == ']')) command[count_command] = CMD_RDX;
						else if ((*(ptr + 4) == 's') & (*(ptr + 5) == 'i') & (*(ptr + 6) == ']')) command[count_command] = CMD_RSI;
						else if ((*(ptr + 4) == 'p') & (*(ptr + 5) == 'i') & (*(ptr + 6) == ']')) command[count_command] = CMD_RPI;
						else
						{
							printf("Invalid registr - %s, line is %d. Maybe you forget \"]\"\n", lines[i], count_strings + 1);
							num_errors++;
						}
					}
					else
					{
						num_errors++;
						printf("Invalid push argument %d - %s\n", count_strings + 1, lines[i]);

						count_command--;
					}
				}
				else if (*(ptr) == 'r')
				{
					command[count_command - 1] = CMD_PUSH_RAM;
					if ((*(ptr + 1) == 'a') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RAX;
					else if ((*(ptr + 1) == 'b') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RBX;
					else if ((*(ptr + 1) == 'c') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RCX;
					else if ((*(ptr + 1) == 'd') & (*(ptr + 2) == 'x')) command[count_command] = CMD_RDX;
					else if ((*(ptr + 1) == 's') & (*(ptr + 2) == 'i')) command[count_command] = CMD_RSI;
					else if ((*(ptr + 1) == 'p') & (*(ptr + 2) == 'i')) command[count_command] = CMD_RPI;
					else
					{
						printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);
						num_errors++;
					}
				}
				else if (*(ptr) == ']')
				{
					command[count_command - 1] = CMD_PUSH_RAM;
				}
				else
				{
					num_errors++;
					printf("Invalid push argument %d - %s\n", count_strings + 1, lines[i]);

					count_command--;
				}

				printf("FIND\n");
			}
			else
			{
				num_errors++;
				printf("Invalid push argument %d - %s\n", count_strings + 1, lines[i]);

				count_command--;
			}
			count_command++;
			count_command++;
		}
*/
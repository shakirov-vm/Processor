#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include "Enum.h"
#include "Assembler.h"

// Для условных джампов числа для сравнения должны лежать в регистрах rbx и rcx
// Условные джампы срабатывают тогда, когда условие выполнено (т.е. je сработает, если rbx = rcx)
// НУЖНО СТАВИТЬ РОВНО ОДИН РАЗДЕЛИТЕЛЬ МЕЖДУ PUSH И АРГУМЕНТОМ. ЕСЛИ ЭТО НЕ ТАК - UNKNOWN COMMAND. Если неверно написано имя регистра
// - то выдаст ошибку неверного названия регистра.

void Assembler(char* input, char* output)      // line is word, string is string
{
	unsigned long size_of_input = KnowFileSize(input);

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));
	double* command = (double*)calloc(size_of_input, sizeof(int));
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
	//Проблема с последними словами (мусор после)    Is it EXIST?

	lines[0] = assembliruemoe;    // Разбиения на, по сути, слова
	size_t count_lines = 1;

	for (int i = 0; i < size_of_input; i++) 
	{
		if (isspace(assembliruemoe[i]))	
		{
			lines[count_lines] = assembliruemoe + i + 1;                               //Вынести в функцию
			count_lines++;
			assembliruemoe[i] = '\0';
		}
	}

	size_t count_command = 0;															//Нужен ли? - да

	char** tags = (char**)calloc(count_lines, sizeof(char*));
	size_t count_tags = 0;
	int* place = (int*)calloc(count_lines, sizeof(int));

	size_t num_errors = 0;

	/*for (int i = 0; i < count_lines; i++)
	{
		printf("%d - [%s]\n", i, lines[i]);
	}*/

	for (int i = 0; i < count_lines; i++) // continue в конце каждого if
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
				tags[count_tags] = lines[i];
				place[count_tags] = count_command;    //                               КАК СТОИТ РЕАЛИЗОВАТЬ??
				count_tags++;
			}
		}
	}
//=========================================================================================================================================
	count_command = 0;                                                              //         COPYPASTE
	count_strings = 0;
//=========================================================================================================================================
	char* ptr;

	for (int i = 0; i < count_lines; i++) // continue в конце каждого if
	{
		//printf("%d - [%s]\n", count_command, lines[i]);

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
			else if (*(ptr) == '[')  // Это для оперативки и видеопамяти
			{
				printf("FIND\n");
			}
			else
			{
				num_errors++;
				printf("Invalid push argument %d - %s\n", count_strings + 1, lines[i]);

				count_command--;
			}
			count_command++;
		}
		else if (strcmp(lines[i], "pop") == 0)
		{
			double argument = strtod(lines[i + 1], &ptr);
			count_command++;
			i++;

			if (*ptr == '\0')
			{
				command[count_command - 1] = CMD_POP;
				command[count_command] = argument;
			}

			else if (*(ptr) == 'r')
			{
				command[count_command - 1] = CMD_POP_R;
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
			else if (*(ptr) == '[')  // Это для оперативки и видеопамяти
			{
				printf("FIND\n");
			}
			else
			{
				num_errors++;
				printf("Invalid pop argument %d - %s\n", count_strings + 1, lines[i]);

				count_command--;
			}
			count_command++;
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
		JUMPS("je", JE)      //            Хочется define
		else if (strcmp(lines[i], "jne") == 0)																\
		{																									\
			command[count_command] = CMD_JNE;																\
			count_command++;																				\
			i++;																							\
																											\
			int find = 0;																					\
																											\
			for (int j = 0; j < count_tags; j++)															\
			{																								\
				if (strcmp_my(lines[i], tags[j]) == 0)														\
				{																							\
					command[count_command] = place[j];														\
					count_command++;																		\
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
		else if (strcmp(lines[i], "") == 0);         // Do nothing
		else if ((lines[i + 1][-2] == ':'));
		else 
		{
			if (strcmp(strings[count_strings], ""))
			{
				num_errors++;
				printf("Unknown command on line %d - %s\n", count_strings + 1, lines[i]);
				// Промахивается на строку, если строка начинается не с ошибочного слова, а с нормального (помечает следующую)
			}
		}
		if (strings[count_strings] < lines[i]) count_strings++;
	}

	if (num_errors != 0)
	{
		printf("You made %d syntax errors\n", num_errors);
		free(assembliruemoe);
		free(lines);
		free(command);
	}
	assert(!num_errors);
	// RETURN ERROR?

	/*for (int i = 0; i < count_strings; i++)
	{
		printf("%s\n", strings[i]);
	}*/

	BinaryWriter(output, count_command, count_lines, command);

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
	/*printf("<<< %d %d\n", count_command, count_lines);
	for (int i = 0; i < count_command; i++)
	{
		printf("%.0lf ", command[i]);
	}*/
	printf("\n");

	int entered = fwrite(command, sizeof(double), count_command, potok);

	//printf(">>> %d\n", entered);

	fclose(potok);
}

unsigned long KnowFileSize(char* input)
{
	struct stat buff;
	stat(input, &buff);

	return buff.st_size;
}


//================================================================================================
//Код для исправления:
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
		// Немного сомнительно

		if (*(ptr + 1) == 'a') command[count_command] = CMD_RAX;
		if (strcmp(lines[i + 1], "rbx")) command[count_command] = CMD_RBX;
		if (strcmp(lines[i + 1], "rcx")) command[count_command] = CMD_RCX;
		if (strcmp(lines[i + 1], "rdx")) command[count_command] = CMD_RDX;
		if (strcmp(lines[i + 1], "rsi")) command[count_command] = CMD_RSI;
		if (strcmp(lines[i + 1], "rpi")) command[count_command] = CMD_RPI;
	}
	else if (*(ptr) == '[')  // Это для оперативки и видеопамяти
	{

	}
	else
	{
		num_errors++;
		printf("Invalid push argument %d\n", count_strings + 1);
	}
	count_command++;
}
else if (strcmp(lines[i], "pop") == 0)
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
}*/

// Ещё рабочий jump ===============================================================================================

/*
		else if (strcmp(lines[i], "jb") == 0)
		{
			command[count_command] = CMD_JB;
			count_command++;
			i++;
			                                                            ПРОВЕРКА РАБОТАЕТ НЕВЕРНО
			if (strcmp(lines[i], ""))
			{
				num_errors++;
				printf("Invalid jump argument %d\n", count_strings + 1);
			}

			for (int j = 0; j < count_tags; j++)
			{
				if (strcmp_my(lines[i], tags[j]) == 0)
				{
					command[count_command] = place[j];
					count_command++; 
					break;
				}
			}
		}*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#ifndef ENUM_H    
#define ENUM_H  
#include "Enum.h"
#endif 

#ifndef ASSEMBLER_H    
#define ASSEMBLER_H  
#include "Assembler.h"
#endif 

// For conditional jumps, the numbers for comparison must be in the rbx and rcx registers
// Conditional jumps are triggered when the condition (i.e. je is triggered if rbx = rcx)
// YOU MUST PUT EXACTLY ONE SEPARATOR BETWEEN PUSH AND ARGUMENT. IF IT'S NOT SO - UNKNOWN TEAM. If the register name is spelled incorrectly
// - it will give an error with an incorrect register name. 

class String
{
public:
	char** strings;
	size_t count_strings;

	String(size_t size_of_input, char** assembliruemoe);
	~String();
	const String& operator= (const String& strs) = delete;
	String(const String& strs) = delete;
};

String::String(size_t size_of_input, char** assembliruemoe)
{
	char** strings = (char**) calloc(size_of_input, sizeof(char*));
	size_t count_strings = 1;
	strings[0] = *assembliruemoe;
	for (int i = 0; i < size_of_input; i++)
	{
		if ((*assembliruemoe)[i] == '\n')
		{
			strings[count_strings] = *assembliruemoe + i + 1;
			count_strings++;
		}
	}
}
String::~String()
{
	printf("FREE\n");
	free(strings);
}

void Assembler(char* input, char* output)    
{
	size_t size_of_input = KnowFileSize(input);

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));

	class Cmd CMD(size_of_input);
	
	char** lines = (char**)calloc(size_of_input, sizeof(char*));

	FileReader(input, assembliruemoe, size_of_input);

	class String strs(size_of_input, &assembliruemoe);
/*
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
*/
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

	for (int i = 0; i < count_lines; i++)
	{
		printf("[%s]\n", lines[i]);
	}

	class Labels lbl(count_lines);

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
	strs.count_strings = 0;
//=========================================================================================================================================
	char* ptr;

	for (int i = 0; i < count_lines; i++) 
	{
		//printf("%ld - [%s]\n", CMD.count_command, lines[i]);
		printf("BEFORE PUSH\n");
		//printf("[%s] - {%p}\n", lines[i], lines + i);
		PUSH_HAND
		POP_HAND
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
		JUMPS("je", JE)  
		JUMPS("jne", JNE)

		else if (strcmp(lines[i], "") == 0);        
 		else if ((lines[i + 1][-2] == ':'));
		else 
		{
			if (strcmp(strs.strings[strs.count_strings], ""))
			{
				num_errors++;
				printf("Unknown command on line %ld - %s\n", strs.count_strings + 1, lines[i]);
			}
		}
		if (strs.strings[strs.count_strings] < lines[i]) strs.count_strings++;
	}

	if (num_errors != 0)
	{
		printf("You made %ld syntax errors\n", num_errors);
		free(assembliruemoe);
		free(lines);
	}

	BinaryWriter(output, CMD.count_command, count_lines, CMD.command);

	assert(!num_errors);															// Return BACK

	free(assembliruemoe);
	free(lines);
}

void FileReader(char* input, char* assembliruemoe, size_t size_of_input)
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

void BinaryWriter(char* output, size_t count_command, size_t count_lines, double* command)
{
	FILE* potok = fopen(output, "wb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Assembler: Problem file: %s\n", output);

		perror(answer);
		exit(2);
	}
	printf("<<< %ld %ld\n", count_command, count_lines);
	for (int i = 0; i < count_command; i++)
	{
		printf("%.0lf ", command[i]);
	}
	printf("\n");

	int entered = fwrite(command, sizeof(double), count_command, potok);

	fclose(potok);
}

size_t KnowFileSize(char* input)
{
	struct stat buff;
	stat(input, &buff);

	return buff.st_size;
}

void CountInc(size_t* count_command, int* i, int cc_inc, int i_inc)
{														
	if (count_command != nullptr)	*count_command = *count_command + cc_inc;			
	if (i != nullptr) 				*i = *i + i_inc;	
}

int JumpHandler (struct Cmd* CMD, struct Labels* lbl, int* i, size_t* num_errors, char*** lines, class String* strs)
{
	CMD->count_command++;																			
	(*i)++;																								

	int find = 0;																					
																										
	for (int j = 0; j < lbl->count_tags; j++)															
	{		
		//			  AWFUL CONSTRUCTION
		if (strcmp_my(*(*(lines) + (*i)), lbl->tags[j]) == 0)														
		{																								
			CMD->command[CMD->count_command] = lbl->place[j];												
			CMD->count_command++;																		
			find++;																						
																					
			return 1;																						
		}																								
	}																								
	if (find == 0)																					
	{																								
		printf("This tag - <%s> - is unknown. Error. Line is %ld\n", (*(*(lines) + (*i))), strs->count_strings + 1);	
		num_errors++;																				
	}
	return 0;
}

void PushHandle(struct Cmd* CMD, int* i, size_t* num_errors, char*** lines, class String* strs)
{
	char* ptr;
	double argument = strtod(*(*(lines) + (*i) + 1), &ptr);
	CMD->count_command++;
	(*i)++;

	if (*ptr == '\0')
	{
		CMD->command[CMD->count_command - 1] = CMD_PUSH;
		CMD->command[CMD->count_command] = argument;
		CMD->count_command++;
	}
	else if (*(ptr) == 'r')
	{
		CMD->command[CMD->count_command - 1] = CMD_PUSH_R;
		REG_FILL(1, 2)	
		else
		{
			printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
			*num_errors++;
		}
		CMD->count_command++;
	}
	else if (*(ptr) == '[') 
	{
		if (*(ptr + 1) == 'r')
		{
			CMD->command[CMD->count_command - 1] = CMD_PUSH_RAM;

			REG_FILL(2, 3)	
			else
			{
				printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
				*num_errors++;
			}

			if (*(ptr + 4) == ']')
			{
				CMD->command[CMD->count_command - 1] = CMD_PUSH_RAM;
				CMD->command[CMD->count_command + 1] = 0;
				CMD->count_command = CMD->count_command + 2;                                                   
			}
			else if (*(ptr + 5) == '+')
			{
				if (isdigit(*(ptr + 7)))
				{
					argument = strtod (ptr + 7, &ptr);

					if (*(ptr) == ']')
					{
						CMD->command[CMD->count_command - 1] = CMD_PUSH_RAM;
						CMD->command[CMD->count_command + 1] = argument;
						CMD->count_command = CMD->count_command + 2;
					}
					else 
					{
						printf("Error in [rax + num] - you forgot <<]>>. It's on line %ld\n", strs->count_strings + 1);
						*num_errors++;
					}
				}
				else
				{
					printf("Error in [rax + num] - waiting number. It's on line %ld\n", strs->count_strings + 1);
					*num_errors++;
				}
			}
			else
			{
				printf("Error in [rax]. It's on line %ld\n", strs->count_strings + 1);
				*num_errors++;
			}
		}
		else if (isdigit(*(ptr + 1)))	
		{	
			argument = strtod(ptr + 1, &ptr);

			if (*(ptr) == ']')
			{
				CMD->command[CMD->count_command - 1] = CMD_PUSH_RAM;
				CMD->command[CMD->count_command - 0] = CMD_NOT_REG;
				CMD->command[CMD->count_command + 1] = argument;
				CMD->count_command = CMD->count_command + 2;
			}
			else
			{
				printf("Error in [num]. It's on line %ld\n", strs->count_strings + 1);
				*num_errors++;
			}
		}
		else
		{
			printf("Error in []. It's on line %ld\n", strs->count_strings + 1);
			*num_errors++;
		}
	}
	else
	{
		*num_errors++;
		printf("Invalid push argument %ld - %s\n", strs->count_strings + 1, *(*(lines) + (*i)));

		CMD->count_command--;
	}
}

void PopHandle(struct Cmd* CMD, int* i, size_t* num_errors, char*** lines, class String* strs)
{
	char* ptr;
	double argument = strtod(*(*(lines) + (*i) + 1), &ptr);
	CMD->count_command++;
	(*i)++;

	if (*ptr == '\0')
	{
		CMD->command[CMD->count_command - 1] = CMD_POP;
		CMD->command[CMD->count_command] = argument;
		CMD->count_command++;
	}
	else if (*(ptr) == 'r')
	{
		CMD->command[CMD->count_command - 1] = CMD_POP_R;
		REG_FILL(1, 2)	
		else
		{
			printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
			*num_errors++;
		}
		CMD->count_command++;
	}
	else if (*(ptr) == '[') 
	{
		if (*(ptr + 1) == 'r')
		{
			CMD->command[CMD->count_command - 1] = CMD_POP_RAM;

			REG_FILL(2, 3)	
			else
			{
				printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
				*num_errors++;
			}

			if (*(ptr + 4) == ']')
			{
				CMD->command[CMD->count_command - 1] = CMD_POP_RAM;
				CMD->command[CMD->count_command + 1] = 0;
				CMD->count_command = CMD->count_command + 2;                                                   
			}
			else if (*(ptr + 5) == '+')
			{
				if (isdigit(*(ptr + 7)))
				{
					argument = strtod (ptr + 7, &ptr);

					if (*(ptr) == ']')
					{
						CMD->command[CMD->count_command - 1] = CMD_POP_RAM;
						CMD->command[CMD->count_command + 1] = argument;
						CMD->count_command = CMD->count_command + 2;
					}
					else 
					{
						printf("Error in [rax + num] - you forgot <<]>>. It's on line %ld\n", strs->count_strings + 1);
						*num_errors++;
					}
				}
				else
				{
					printf("Error in [rax + num] - waiting number. It's on line %ld\n", strs->count_strings + 1);
					*num_errors++;
				}
			}
			else
			{
				printf("Error in [rax]. It's on line %ld\n", strs->count_strings + 1);
				*num_errors++;
			}
		}
		else if (isdigit(*(ptr + 1)))	
		{	
			argument = strtod(ptr + 1, &ptr);

			if (*(ptr) == ']')
			{
				CMD->command[CMD->count_command - 1] = CMD_POP_RAM;
				CMD->command[CMD->count_command - 0] = CMD_NOT_REG;
				CMD->command[CMD->count_command + 1] = argument;
				CMD->count_command = CMD->count_command + 2;
			}
			else
			{
				printf("Error in [num]. It's on line %ld\n", strs->count_strings + 1);
				*num_errors++;
			}
		}
		else
		{
			printf("Error in []. It's on line %ld\n", strs->count_strings + 1);
			*num_errors++;
		}
	}
	else
	{
		*num_errors++;
		printf("Invalid push argument %ld - %s\n", strs->count_strings + 1, *(*(lines) + (*i)));

		CMD->count_command--;
	}
}
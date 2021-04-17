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


void Cmd::PushHandle(int* i, size_t* num_errors, char*** lines, class String* strs)
{
	char* ptr;
	double argument = strtod(*(*(lines) + (*i) + 1), &ptr);
	count_command++;
	(*i)++;

	if (*ptr == '\0')
	{
		command[count_command - 1] = CMD_PUSH;
		command[count_command] = argument;
		count_command++;
	}
	else if (*(ptr) == 'r')
	{
		command[count_command - 1] = CMD_PUSH_R;
		REG_FILL(1, 2)	
		else
		{
			printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
			*num_errors++;
		}
		count_command++;
	}
	else if (*(ptr) == '[') 
	{
		if (*(ptr + 1) == 'r')
		{
			command[count_command - 1] = CMD_PUSH_RAM;

			REG_FILL(2, 3)	
			else
			{
				printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
				*num_errors++;
			}

			if (*(ptr + 4) == ']')
			{
				command[count_command - 1] = CMD_PUSH_RAM;
				command[count_command + 1] = 0;
				count_command = count_command + 2;                                                   
			}
			else if (*(ptr + 5) == '+')
			{
				if (isdigit(*(ptr + 7)))
				{
					argument = strtod (ptr + 7, &ptr);

					if (*(ptr) == ']')
					{
						command[count_command - 1] = CMD_PUSH_RAM;
						command[count_command + 1] = argument;
						count_command = count_command + 2;
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
				command[count_command - 1] = CMD_PUSH_RAM;
				command[count_command - 0] = CMD_NOT_REG;
				command[count_command + 1] = argument;
				count_command = count_command + 2;
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

		count_command--;
	}
}

void Cmd::PopHandle(int* i, size_t* num_errors, char*** lines, class String* strs)
{
	char* ptr;
	double argument = strtod(*(*(lines) + (*i) + 1), &ptr);
	count_command++;
	(*i)++;

	if (*ptr == '\0')
	{
		command[count_command - 1] = CMD_POP;
		command[count_command] = argument;
		count_command++;
	}
	else if (*(ptr) == 'r')
	{
		command[count_command - 1] = CMD_POP_R;
		REG_FILL(1, 2)	
		else
		{
			printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
			*num_errors++;
		}
		count_command++;
	}
	else if (*(ptr) == '[') 
	{
		if (*(ptr + 1) == 'r')
		{
			command[count_command - 1] = CMD_POP_RAM;

			REG_FILL(2, 3)	
			else
			{
				printf("Invalid registr - %s, line is %ld\n", *(*(lines) + (*i)), strs->count_strings + 1);
				*num_errors++;
			}

			if (*(ptr + 4) == ']')
			{
				command[count_command - 1] = CMD_POP_RAM;
				command[count_command + 1] = 0;
				count_command = count_command + 2;                                                   
			}
			else if (*(ptr + 5) == '+')
			{
				if (isdigit(*(ptr + 7)))
				{
					argument = strtod (ptr + 7, &ptr);

					if (*(ptr) == ']')
					{
						command[count_command - 1] = CMD_POP_RAM;
						command[count_command + 1] = argument;
						count_command = count_command + 2;
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
				command[count_command - 1] = CMD_POP_RAM;
				command[count_command - 0] = CMD_NOT_REG;
				command[count_command + 1] = argument;
				count_command = count_command + 2;
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

		count_command--;
	}
}
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

size_t LinesFiller(char*** lines, char** assembliruemoe, size_t size_of_input)
{
	**lines = *assembliruemoe;    
	size_t count_lines = 1;

	for (int i = 0; i < size_of_input; i++) 
	{
		if (isspace(*(*assembliruemoe + i)))	
		{
            *(*lines + count_lines) = *assembliruemoe + i + 1;           
			count_lines++;
			*(*assembliruemoe + i) = '\0';
		}
	}	

	return count_lines; 
}

void Assembler(char* input, char* output)    
{
	size_t size_of_input = KnowFileSize(input);

	char* assembliruemoe = (char*)calloc(size_of_input, sizeof(char));

	class Cmd CMD(size_of_input);
	
	char** lines = (char**)calloc(size_of_input, sizeof(char*));

	FileReader(input, assembliruemoe, size_of_input);

	class String strs (size_of_input, &assembliruemoe);

	size_t count_lines = LinesFiller(&lines, &assembliruemoe, size_of_input);

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
	for (int i = 0; i < count_lines; i++) 
	{
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

	CMD.BinaryWriter(output, count_lines);

	assert(!num_errors);															// Return BACK

	free(assembliruemoe);
	free(lines);
}
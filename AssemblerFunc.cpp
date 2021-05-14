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

Labels::Labels(size_t count_lines)
{
	tags = (char**)calloc(count_lines, sizeof(char*));
	count_tags = 0;
	place = (int*)calloc(count_lines, sizeof(int));
}
Labels::~Labels()
{
	free(tags);
	free(place);
}

Cmd::Cmd(size_t size_of_input)
{
	command = (double*)calloc(size_of_input, sizeof(int));
	count_command = 0;
}
Cmd::~Cmd()
{
	free(command);
}

void Cmd::BinaryWriter(char* output, size_t count_lines)
{
	FILE* potok = fopen(output, "wb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Assembler: Problem file: %s\n", output);

		perror(answer);
		exit(2);
	}
	/*printf("<<< %ld %ld\n", count_command, count_lines);
	for (int i = 0; i < count_command; i++)
	{
		printf("%.0lf ", command[i]);
	}
	printf("\n");*/

	int entered = fwrite(command, sizeof(double), count_command, potok);

	fclose(potok);
}
void Cmd::CountInc(int* i, int cc_inc, int i_inc)
{														
	count_command = count_command + cc_inc;			
	if (i != nullptr) *i = *i + i_inc;	
}
int Cmd::JumpHandler (class Labels* lbl, int* i, size_t* num_errors, char*** lines, class String* strs)
{
	count_command++;																			
	(*i)++;																								

	int find = 0;																					
																										
	for (int j = 0; j < lbl->count_tags; j++)															
	{		
		//			  AWFUL CONSTRUCTION
		if (strcmp_my(*(*(lines) + (*i)), lbl->tags[j]) == 0)														
		{																								
			command[count_command] = lbl->place[j];												
			count_command++;																		
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

String::String(size_t size_of_input, char** assembliruemoe)
{
	strings = (char**) calloc(size_of_input, sizeof(char*));
	count_strings = 1;
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
	free(strings);
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

size_t KnowFileSize(char* input)
{
	struct stat buff;
	stat(input, &buff);

	return buff.st_size;
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include "Assembler.h"
#include "Enum.h"

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

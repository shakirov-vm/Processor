
#include <stdio.h>
#include <stdlib.h>

void Processor(char* input);
void Assembler(char* input, char* output);
void Dissambler(char* input, char* output);

int main(int argc, char* argv[])
{
	//char* input = (char*)calloc(9, sizeof(char));
	//char* output = (char*)calloc(10, sizeof(char));

	Assembler(argv[1], argv[2]);

	Processor(argv[2]);

	Dissambler(argv[2], argv[3]);

	return 0;
}


#include <stdlib.h>

void CPU(char* input);
void FileEnter(char* input, char* output);

int main(int argc, char* argv[])
{
	//char* input = (char*)calloc(9, sizeof(char));
	//char* output = (char*)calloc(10, sizeof(char));

	FileEnter(argv[1], argv[2]);

	CPU(argv[2]);

	return 0;
}

#ifndef PROCESSOR_H    
#define PROCESSOR_H
#include "Processor.h"
#endif 

// OUT WORK WITH RAX. ARGUMENT FROM STACK GO TO RAX
// SQRT TOO

void Processor(char* input)
{
	size_t size_of_input = KnowSizeInput(input);

	class Proc CPU(size_of_input);
	
	int read_out = CPU.CommandFiller(input, size_of_input);

	CPU.stk.DUMP();
	
	CPU.CPUHandle();
}

void Proc::CPUHandle()
{
	for (int i = 0; ; i++)
	{
		switch ((int)commands[IP])
		{
			PROC_OTHER(PUSH_RAM)
			PROC_OTHER(PUSH_R)
			PROC_OTHER(POP_R)
			PROC_OTHER(PUSH)
			PROC_OTHER(OUT)
			PROC_OTHER(CALL)
			PROC_OTHER(RET)
			PROC_OTHER(JMP)
			PROC_OTHER(IN)	
			PROC_OTHER(SQRT)	
			PROC_ARIFM(ADD, +)    
			PROC_ARIFM(MUL, *)
			PROC_ARIFM(DIV, /)
			PROC_ARIFM(SUB, -)
			PROC_JUMPS(JB, <)
			PROC_JUMPS(JBE, <= )
			PROC_JUMPS(JA, > )
			PROC_JUMPS(JAE, >= )
			PROC_JUMPS(JE, == )
			PROC_JUMPS(JNE, != )
			case CMD_END:
			{
				this->CPU_END();
				return;
			}
		}
	}
}


int Proc::CommandFiller(char* input, int size_of_input)
{
	FILE* potok = fopen(input, "rb");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Processor: Problem file: %s\n", input);

		perror(answer);
		exit(1);
	}

	int read_out = fread(commands, sizeof(double), size_of_input, potok);

	printf("\n");
	
	fclose(potok);

	return read_out;
}

Proc::Proc(size_t size_of_input)
{
	IP = 0;
	registr = (double*)calloc(NUM_OF_REG, sizeof(double));
	commands = (double*)calloc(size_of_input, sizeof(double));
	RAM  = (double*)calloc(1024, sizeof(double));
	VRAM = (char*)calloc(1024, sizeof(char));
}

Proc::~Proc()
{
	free(registr);
	free(commands);
	free(RAM);
	free(VRAM);
}

void Proc::CPU_PUSH_RAM()
{
	size_t adress = 0;
	if(((int)commands[IP + 1]) != 110)
	{
		adress = registr[(int)commands[IP + 1] - CMD_RAX] + (int)commands[IP + 2];
	}
	else
	{
		adress = (int)commands[IP + 2];
	}
	IP = IP + 3;

	if ((adress >= 0) && (adress <= 1023))
	{
		RAM[adress] = stk.Pop();
		printf("push from stack to RAM on adress %ld\n", adress);
		this->PrintRAM();
	}
	else if((adress >= 1024) && (adress <= 2047))
	{
		VRAM[adress - 1024] = (char)stk.Pop();
		printf("push from stack to VRAM on adress %ld\n", adress);
		this->PrintVRAM();
	}
	else 
	{
		printf("This adress is invalid - %ld\n", adress);
	}
}

void Proc::CPU_PUSH_R()
{
	stk.Push(registr[(int)commands[IP + 1] - CMD_RAX]);

	if(mode == 1) printf("push out of r%cx number <%.0lf>\n", (int)(commands[IP + 1] - CMD_RAX + 97), registr[(int)commands[IP + 1] - CMD_RAX]);

	IP = IP + 2;

	stk.DUMP();
	reg_ret.DUMP();

}
void Proc::CPU_POP_R()
{
	registr[(int)commands[IP + 1] - CMD_RAX] = stk.Pop();

	if(mode == 1) printf("pop in r%cx number <%.0lf>\n", (int)(commands[IP + 1] - CMD_RAX + 97), registr[(int)commands[IP + 1] - CMD_RAX]);

	IP = IP + 2;
}
void Proc::CPU_PUSH()
{
	stk.Push(commands[IP + 1]);

	if(mode == 1) printf("push %.0lf\n", commands[IP + 1]);

	IP = IP + 2;
}
void Proc::CPU_OUT()
{
	IP++;

	double a = stk.Pop();

	printf("out %lf\n", a);   

}
void Proc::CPU_IN()
{
	IP++;

	double arg = 0;

	int value = 0;
 
	while (value != 1)
	{
		value = scanf("%lf", &arg);
	}
	stk.Push(arg);
}
void Proc::CPU_SQRT()
{
	IP++;

	rax = stk.Pop();

	rax = sqrt(rax);

	stk.Push(rax);
}
void Proc::CPU_CALL()
{
	reg_ret.Push(IP); 

	IP = commands[IP + 1];

	if(mode == 1) printf("CALL ON %d\n",  IP);

	reg_ret.DUMP();
}
void Proc::CPU_RET()
{
	IP = reg_ret.Pop();
	IP = IP + 2;

	if(mode == 1) printf("RET ON %d\n", IP);

	reg_ret.DUMP();
}
void Proc::CPU_JMP()
{
	IP = commands[IP + 1];
}
void Proc::CPU_END()
{
	//this->PrintRAM();
	stk.DUMP();
	printf("end\n");
}
void Proc::PrintRAM()
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%0.lf ", RAM[i]);
	}
	printf("\n");
}
void Proc::PrintVRAM()
{
	for (int i = 0; i < 1024; i++)
	{
		printf("%c ", VRAM[i]);
	}
	printf("\n");
}

size_t KnowSizeInput(char* input)
{
	struct stat buff;
	stat (input, &buff);
	return buff.st_size;
}


#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef STACK_CONST_H    
#define STACK_CONST_H  
#include "StackConst.h"
#endif 

#ifndef ENUM_H
#define ENUM_H
#include "Enum.h"
#endif

#if 1
#define TYPE int
#include "Stack.h"
#undef TYPE
#endif                                                        

#if 1
#define TYPE double
#include "Stack.h"
#undef TYPE
#endif

#define rax registr[0]
#define rbx registr[1]
#define rcx registr[2]
#define rdx registr[3]
#define rsi registr[4]
#define rpi registr[5]

#define mode 0

class Proc
{
public:
	double* commands;
	int IP;
	double* registr;
	Stack_double stk;
	Stack_int reg_ret;
	double* RAM;
	char* VRAM;

	Proc(size_t size_of_input);
	~Proc();
	const Proc& operator= (const Proc& CPU) = delete;
	Proc(const Proc& CPU) = delete;
	int CommandFiller(char* input, int size_of_input);
	void CPUHandle();
	void PrintRAM();
	void PrintVRAM();

	void CPU_PUSH_RAM();
	void CPU_PUSH_R();
	void CPU_POP_R();
	void CPU_PUSH();
	void CPU_OUT();
	void CPU_CALL();
	void CPU_SQRT();
	void CPU_RET();
	void CPU_JMP();
	void CPU_END();
	void CPU_IN();
};

size_t KnowSizeInput(char* input);

#define NUM_OF_REG 6

#define PROC_ARIFM(NAME, ACTION)                                                    \
				case CMD##_##NAME:													\
				{																	\
					IP++;															\
																					\
					double a = stk.Pop();											\
					double b = stk.Pop();											\
																					\
																					\
					a = a ACTION b;													\
																					\
					stk.Push(a);													\
																					\
					break;															\
				}
 
#define PROC_JUMPS(NAME, ACTION)													\
			case CMD##_##NAME:														\
			{																		\
				if (rbx ACTION rcx)	IP = commands[IP + 1];							\
				else IP = IP + 2;													\
				break;																\
			}																		

#define PROC_OTHER(NAME)															\
			case CMD##_##NAME:														\
			{																		\
				this->CPU##_##NAME();												\
				break;																\
			}

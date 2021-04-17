
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
	void CPU_RET();
	void CPU_JMP();
	void CPU_END();
};

size_t KnowSizeInput(char* input);

#define NUM_OF_REG 6

#define PROC_ARIFM(NAME, ACTION)                                                    \
				case CMD##_##NAME:													\
				{																	\
					IP++;															\
																					\
					rax = stk.Pop();												\
					rbx = stk.Pop();												\
																					\
																					\
					rax = rax ACTION rbx;											\
																					\
					stk.Push(rax);													\
																					\
					break;															\
				}
 
#define PROC_JUMP(NAME, ACTION)														\
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

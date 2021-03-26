

#define PROC_ARIFM(NAME, ACTION)                                                    \
				case CMD##_##NAME:													\
				{																	\
					CPU.IP++;														\
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
				if (rbx ACTION rcx)	CPU.IP = CPU.commands[CPU.IP + 1];				\
				else CPU.IP = CPU.IP + 2;											\
				break;																\
			}																		\


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
			
//  <<������� �����������>>
enum COMMANDS
{
	CMD_PUSH = 1,
	CMD_PUSH_R = 11,
	CMD_POP_R = 12,
	CMD_POP  = 2,
	CMD_ADD  = 3,
	CMD_SUB  = 4,
	CMD_MUL  = 5,
	CMD_DIV  = 6,
	CMD_OUT  = 7,

	CMD_END  = 10,
	CMD_RET  = 20, 

	CMD_JMP  = 88,
	CMD_CALL = 89,

	CMD_JB  = 32,
	CMD_JBE = 33,
	CMD_JA  = 34,
	CMD_JAE = 35,
	CMD_JNE = 36,
	CMD_JE  = 37
};

enum REGISTERS
{
	CMD_RAX = 111,
	CMD_RBX = 112,
	CMD_RCX = 113,
	CMD_RDX = 114,
	CMD_RSI = 115,
	CMD_RPI = 116
};
size_t KnowFileSize(char* input);
void FileReader(char* input, char* assembliruemoe, size_t size_of_input);
int strcmp_my(char* line, char* tag);
size_t LinesFiller(char*** lines, char** assembliruemoe, size_t size_of_input);

int JumpHandler (struct Cmd* CMD, struct Labels* lbl, int* i, size_t* num_errors, char*** lines, class String* strs);
void PushHandle(struct Cmd* CMD, int* i, size_t* num_errors, char*** lines, class String* strs);
void PopHandle(struct Cmd* CMD, int* i, size_t* num_errors, char*** lines, class String* strs);

#define COMMANDS(NAME, NUMBER, INUM)							\
	if (strcmp(lines[i], NAME) == 0)							\
	{															\
		CMD.CountInc(&i, NUMBER, INUM);							\
		continue;												\
	}											
#define ARIFMETICAL(NAME_LOW, NAME_HIGH)								\
		else if (strcmp(lines[i], NAME_LOW) == 0)						\
		{																\
		CMD.command[CMD.count_command] = CMD##_##NAME_HIGH;				\
		CMD.CountInc(&i, 1, 0);											\
		continue;														\
		}																		

#define JUMPS(NAME_LOW, NAME_HIGH)																			\
		else if (strcmp(lines[i], NAME_LOW) == 0)															\
		{																									\
		CMD.command[CMD.count_command] = CMD##_##NAME_HIGH;													\
		int brk = CMD.JumpHandler (&lbl, &i, &num_errors, &lines, &strs);									\
		if (brk == 1) continue;																				\
		}

#define REG_FILL(R_X, RX_)																					\
																											\
		 if ((*(ptr + R_X) == 'a') && (*(ptr + RX_) == 'x')) command[count_command] = CMD_RAX;				\
	else if ((*(ptr + R_X) == 'b') && (*(ptr + RX_) == 'x')) command[count_command] = CMD_RBX;				\
	else if ((*(ptr + R_X) == 'c') && (*(ptr + RX_) == 'x')) command[count_command] = CMD_RCX;				\
	else if ((*(ptr + R_X) == 'd') && (*(ptr + RX_) == 'x')) command[count_command] = CMD_RDX;				\
	else if ((*(ptr + R_X) == 's') && (*(ptr + RX_) == 'i')) command[count_command] = CMD_RSI;				\
	else if ((*(ptr + R_X) == 'p') && (*(ptr + RX_) == 'i')) command[count_command] = CMD_RPI;

#define PUSH_HAND																							\
	if (strcmp(lines[i], "push") == 0)																		\
	{																										\
		CMD.PushHandle(&i, &num_errors, &lines, &strs);														\
		continue;																							\
	}

#define POP_HAND																							\
	if (strcmp(lines[i], "pop") == 0)																		\
	{																										\
		CMD.PopHandle(&i, &num_errors, &lines, &strs);														\
		continue;																							\
	}

class Labels
{
public:

	char** tags;
	size_t count_tags;
	int* place;

	Labels(size_t count_lines);
	~Labels();
	const Labels& operator= (const Labels& lbl) = delete;
	Labels(const Labels& lbl) = delete;

};

class Cmd
{
public:

	double* command;
	size_t count_command;

	Cmd(size_t size_of_input);
	~Cmd();
	const Cmd& operator= (const Cmd& CMD) = delete;
	Cmd(const Cmd& CMD) = delete;
	void BinaryWriter(char* output, size_t count_lines);
	void CountInc(int* i, int cc_inc, int i_inc);
	int JumpHandler (class Labels* lbl, int* i, size_t* num_errors, char*** lines, class String* strs);
	void PushHandle(int* i, size_t* num_errors, char*** lines, class String* strs);
	void  PopHandle(int* i, size_t* num_errors, char*** lines, class String* strs);
};

class String
{
public:
	char** strings;
	size_t count_strings;

	String(size_t size_of_input, char** assembliruemoe);
	~String();
	const String& operator= (const String& strs) = delete;
	String(const String& strs) = delete;
};
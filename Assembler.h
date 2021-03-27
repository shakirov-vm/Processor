unsigned long KnowFileSize(char* input);
void FileReader(char* input, char* assembliruemoe, int size_of_input);
void BinaryWriter(char* output, int count_command, int count_lines, double* command);
int strcmp_my(char* line, char* tag);

#define COMMANDS(NAME, NUMBER, INUM)							\
	if (strcmp(lines[i], NAME) == 0)							\
	{															\
		CMD.count_command = CMD.count_command + NUMBER;			\
		i = i + INUM;											\
		continue;												\
	}											
#define ARIFMETICAL(NAME_LOW, NAME_HIGH)								\
		else if (strcmp(lines[i], NAME_LOW) == 0)						\
		{																\
		CMD.command[CMD.count_command] = CMD##_##NAME_HIGH;				\
		CMD.count_command++;											\
		continue;														\
		}
#define JUMPS(NAME_LOW, NAME_HIGH)											    \
		else if (strcmp(lines[i], NAME_LOW) == 0)								\
		{																		\
			CMD.command[CMD.count_command] = CMD##_##NAME_HIGH;					\
			CMD.count_command++;												\
			i++;																\
																				\
			for (int j = 0; j < lbl.count_tags; j++)							\
			{																	\
				if (strcmp_my(lines[i], lbl.tags[j]) == 0)						\
				{																\
					CMD.command[CMD.count_command] = lbl.place[j];					\
					CMD.count_command++;										\
					break;														\
				}																\
			}																	\
			continue;															\
		}																		
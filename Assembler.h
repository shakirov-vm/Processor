unsigned long KnowFileSize(char* input);
void FileReader(char* input, char* assembliruemoe, int size_of_input);
void BinaryWriter(char* output, int count_command, int count_lines, double* command);
int strcmp_my(char* line, char* tag);

#define COMMANDS(NAME, NUMBER, INUM)							\
	if (strcmp(lines[i], NAME) == 0)							\
	{															\
		count_command = count_command + NUMBER;					\
		i = i + INUM;											\
		continue;												\
	}											
#define ARIFMETICAL(NAME_LOW, NAME_HIGH)								\
		else if (strcmp(lines[i], NAME_LOW) == 0)						\
		{																\
		command[count_command] = CMD##_##NAME_HIGH;						\
		count_command++;												\
		}
#define JUMPS(NAME_LOW, NAME_HIGH)											    \
		else if (strcmp(lines[i], NAME_LOW) == 0)								\
		{																		\
			command[count_command] = CMD##_##NAME_HIGH;							\
			count_command++;													\
			i++;																\
																				\
			for (int j = 0; j < count_tags; j++)								\
			{																	\
				if (strcmp_my(lines[i], tags[j]) == 0)							\
				{																\
					command[count_command] = place[j];							\
					count_command++;											\
					break;														\
				}																\
			}																	\
		}																		


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
//#define JUMPS(NAME_LOW, NAME_HIGH)											    \
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
					CMD.command[CMD.count_command] = lbl.place[j];				\
					CMD.count_command++;										\
					break;														\
				}																\
			}																	\
			continue;															\
		}																		

//#define STACK_CMD(NAME_LOW, NAME_HIGH)																					\
		if (strcmp(lines[i], NAME_LOW) == 0)																				\
		{																													\
			double argument = strtod(lines[i + 1], &ptr);																	\
			CMD.count_command++;																							\
			i++;																											\
																															\
			if (*ptr == '\0')																								\
			{																												\
				CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH;														\
				CMD.command[CMD.count_command] = argument;																	\
				CMD.count_command++;																						\
			}																												\
			else if (*(ptr) == 'r')																							\
			{																												\
				CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH##_##R;												\
					 if ((*(ptr + 1) == 'a') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RAX				\
				else if ((*(ptr + 1) == 'b') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RBX;				\
				else if ((*(ptr + 1) == 'c') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RCX;				\
				else if ((*(ptr + 1) == 'd') && (*(ptr + 2) == 'x')) CMD.command[CMD.count_command] = CMD_RDX;				\
				else if ((*(ptr + 1) == 's') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RSI;				\
				else if ((*(ptr + 1) == 'p') && (*(ptr + 2) == 'i')) CMD.command[CMD.count_command] = CMD_RPI;				\
				else																										\
				{																											\
					printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);								\
					num_errors++;																							\
				}																											\
				CMD.count_command++;																						\
			}																												\
			else if (*(ptr) == '[') 																						\
			{																												\
				if (*(ptr + 1) == 'r')																						\
				{																											\
					CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH##_##RAM;											\
																															\
						 if ((*(ptr + 2) == 'a') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RAX;			\
					else if ((*(ptr + 2) == 'b') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RBX;			\
					else if ((*(ptr + 2) == 'c') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RCX;			\
					else if ((*(ptr + 2) == 'd') && (*(ptr + 3) == 'x')) CMD.command[CMD.count_command] = CMD_RDX;			\
					else if ((*(ptr + 2) == 's') && (*(ptr + 3) == 'i')) CMD.command[CMD.count_command] = CMD_RSI;			\
					else if ((*(ptr + 2) == 'p') && (*(ptr + 3) == 'i')) CMD.command[CMD.count_command] = CMD_RPI;			\
					else																									\
					{																										\
						printf("Invalid registr - %s, line is %d\n", lines[i], count_strings + 1);							\
						num_errors++;																						\
					}																										\
																															\
					if (*(ptr + 4) == ']')																					\
					{																										\
						CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH##_##RAM;										\
						CMD.command[CMD.count_command + 1] = 0;																\
						CMD.count_command = CMD.count_command + 2;                                                   		\
					}																										\
					else if (*(ptr + 5) == '+')																				\
					{																										\
						if (isdigit(*(ptr + 7)))																			\
						{																									\
							argument = strtod (ptr + 7, &ptr);																\
																															\
							if (*(ptr) == ']')																				\
							{																								\
								CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH##_##RAM;								\
								CMD.command[CMD.count_command + 1] = argument;												\
								CMD.count_command = CMD.count_command + 2;													\
							}																								\
							else 																							\
							{																								\
								printf("Error in [rax + num] - you forgot <<]>>. It's on line %d\n", count_strings + 1);	\
								num_errors++;																				\
							}																								\
						}																									\
						else																								\
						{																									\
							printf("Error in [rax + num] - waiting number. It's on line %d\n", count_strings + 1);			\
							num_errors++;																					\
						}																									\
					}																										\
					else																									\
					{																										\
						printf("Error in [rax]. It's on line %d\n", count_strings + 1);										\
						num_errors++;																						\
					}																										\
				}																											\
				else if (isdigit(*(ptr + 1)))																				\
				{																											\
					argument = strtod(ptr + 1, &ptr);																		\
																															\
					if (*(ptr) == ']')																						\
					{																										\
						CMD.command[CMD.count_command - 1] = CMD##_##NAME_HIGH##_##RAM;										\
						CMD.command[CMD.count_command - 0] = CMD_NOT_REG;													\
						CMD.command[CMD.count_command + 1] = argument;														\
						CMD.count_command = CMD.count_command + 2;															\
					}																										\
					else																									\
					{																										\
						printf("Error in [num]. It's on line %d\n", count_strings + 1);										\
						num_errors++;																						\
					}																										\
				}																											\
				else																										\
				{																											\
					printf("Error in []. It's on line %d\n", count_strings + 1);											\
					num_errors++;																							\
				}																											\
			}																												\
			else																											\
			{																												\
				num_errors++;																								\
				printf("Invalid stack's function argument %d - %s\n", count_strings + 1, lines[i]);							\
																															\
				CMD.count_command--;																						\
			}																												\
		}																													

#define JUMPS(NAME_LOW, NAME_HIGH)																			\
		else if (strcmp(lines[i], NAME_LOW) == 0)															\
		{																									\
		CMD.command[CMD.count_command] = CMD##_##NAME_HIGH;													\
		CMD.count_command++;																				\
		i++;																								\
																											\
		int find = 0;																						\
																											\
		for (int j = 0; j < lbl.count_tags; j++)															\
		{																									\
			if (strcmp_my(lines[i], lbl.tags[j]) == 0)														\
			{																								\
				CMD.command[CMD.count_command] = lbl.place[j];												\
				CMD.count_command++;																		\
				find++;																						\
																											\
				break;																						\
			}																								\
		}																									\
			if (find == 0)																					\
			{																								\
				printf("This tag - <%s> - is unknown. Error. Line is %d\n", lines[i], count_strings + 1);	\
				num_errors++;																				\
			}																								\
		}








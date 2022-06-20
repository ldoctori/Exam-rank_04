all:
	gcc microshell.c executer.c get_command_list.c -o microshell -Wall -Wextra -Werror

#ifndef MICROSHELL_H
#define MICROSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/wait.h>
#include <signal.h>

typedef struct s_cmd
{
    int     pipe_in, pipe_out;
    char    **cmd_args;
    int     cmd_number;
    int     args_num;
    char    **envp;
    struct s_cmd    *next;
}   t_cmd;

int     ft_strlen(char *str);
t_cmd   *get_command_list(char **argv, char **envp);
void    free_cmd_list(t_cmd *cmd);
void    executer(t_cmd *cmd);
void    exit_fatal();
void    cd(int argc, char **argv);

#endif
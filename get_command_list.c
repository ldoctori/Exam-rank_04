#include "microshell.h"

t_cmd   *get_command(char **argv, char **envp, int args_num)
{
    if (args_num == 0)
        return (NULL);
    t_cmd   *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        exit_fatal();
    if (strcmp(*argv, "|") == 0)
    {
        cmd->pipe_in = 1;
        argv++;
    }
    else
        cmd->pipe_in = 0;
    cmd->cmd_args = malloc(sizeof(char *) * (args_num + 1));
    int i = 0;
     while (*argv && (strcmp(*argv, "|") == 0 || strcmp(*argv, ";") == 0))
        argv++;
    while (*argv && (strcmp(*argv, "|") != 0 && strcmp(*argv, ";") != 0))
    {
        cmd->cmd_args[i] = *argv;
        argv++;
        i++;
    }
    cmd->cmd_args[i] = NULL;
    cmd->next = NULL;
    cmd->envp = envp;
    if (*argv && strcmp(*argv, "|") == 0)
        cmd->pipe_out = 1;
    else
        cmd->pipe_out = 0;
    cmd->args_num = args_num;
    return (cmd);
}

int get_args_num(char **argv)
{
    int i = 0;
    while (*argv && (strcmp(*argv, "|") == 0 || strcmp(*argv, ";") == 0))
        argv++;
    while (*argv && (strcmp(*argv, "|") != 0 && strcmp(*argv, ";") != 0))
    {
        i++;
        argv++;
    }
    return (i);
}

t_cmd   *get_command_list(char **argv, char **envp)
{
    t_cmd   *cmd, *start;
    int     args_num;

    args_num = get_args_num(argv);
    start = cmd = get_command(argv, envp, args_num);
    if (!start)
        return (NULL);
    argv += args_num;
    while (*argv)
    {
        args_num = get_args_num(argv);
        cmd->next = get_command(argv, envp, args_num);
        cmd = cmd->next;
        while (*argv && ( strcmp(*argv, "|") == 0 || strcmp(*argv, ";") == 0))
            argv++;
        argv += args_num;
    }
        return (start);
}

void    free_cmd_list(t_cmd *cmd)
{
    t_cmd   *tmp;

    while (cmd)
    {
        free(cmd->cmd_args);
        tmp = cmd;
        cmd = cmd->next;
        free(tmp);
    }
}
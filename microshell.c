#include "microshell.h"

int ft_strlen(char *str)
{
    int n = 0;

    while (str[n])
        n++;
    return (n);
}

void    set_number_of_cmd(t_cmd *cmd)
{
    t_cmd   *tmp = cmd;
    int     n = 0;

    while (tmp)
    {
        n++;
        tmp = tmp->next;
    }
    while (cmd)
    {
        cmd->cmd_number = n;
        cmd = cmd->next;
    }
}

void    cd(int argc, char **argv)
{
    if (argc != 2)
        write(2, "error: cd: bad arguments\n", 25);
    else if (chdir(argv[1]) == -1)
    {
        write(2, "error: cd: cannot change directory to ", 38);
        write(2, argv[1], ft_strlen(argv[1]));
        write(2, "\n", 1);
    }
}

int main(int argc, char **argv, char **envp)
{
    if (argc == 1)
        return (0);
    
    t_cmd   *cmd = get_command_list(argv + 1, envp);
    if (!cmd)
        return (0);
    set_number_of_cmd(cmd);
    executer(cmd);
    free_cmd_list(cmd);
}
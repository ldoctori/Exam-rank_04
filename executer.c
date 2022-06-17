#include "microshell.h"

void    exit_fatal()
{
    write(2, "error: fatal\n", 13);
    exit(EXIT_FAILURE);
}

int **fd_allocate(int n)
{
    int **fd = malloc(sizeof(int *) * n);
    int i = 0;

    while (i < n - 1)
    {
        fd[i] = malloc(sizeof(int) * 2);
        if (pipe(fd[i]) == -1)
            exit_fatal();
        i++;
    }
    fd[i] = NULL;
    return (fd);
}

void    free_fd_pid(int **fd, int *pid, int n)
{
    int i = 0;
    while (i < n)
    {
        free(fd[i]);
        i++;
    }
    free(fd);
    free(pid);
}

void    fd_duplicate(int **fd, int i, t_cmd *cmd)
{
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;

    if (i > 0 && cmd->pipe_in == 1)
        in_fd = fd[i - 1][0];
    if (i < cmd->cmd_number && cmd->pipe_out == 1)
        out_fd = fd[i][1];
    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);

    int j = 0;
    while (j < cmd->cmd_number - 1)
    {
        close(fd[j][0]);
        close(fd[j][1]);
        j++;
    }
}

void    close_wait_free(int **fd, int *pid, int n)
{
    int exit_status;

    int i = 0;
    while (i < n - 1)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
    i = 0;
    while (i < n)
    {
        waitpid(pid[i], &exit_status, 0);
        i++;
    }
    free_fd_pid(fd, pid, n);
}

void    executer(t_cmd *cmd)
{
    int **fd;
    int *pid = malloc(sizeof(int) * cmd->cmd_number);
    int i = 0;
    int cmd_number = cmd->cmd_number;

    if (!pid)
        exit_fatal();
    fd = fd_allocate(cmd_number);
    if (!fd)
        exit_fatal();
    while (cmd)
    {
        if (strcmp(cmd->cmd_args[0], "cd") == 0)
            cd(cmd->args_num, cmd->cmd_args);
        else
        {            
            pid[i] = fork();
            if (pid[i] < 0)
                exit_fatal();
            if (pid[i] == 0)
            {
                fd_duplicate(fd, i, cmd);
                if (execve(cmd->cmd_args[0], cmd->cmd_args, cmd->envp) == -1)
                {
                    write(2, "cannot execute ", 15);
                    write(2, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
                    write(2, "\n", 1);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                waitpid
            }
            i++;
        }
        cmd = cmd->next;
    }
    close_wait_free(fd, pid, cmd_number);
}
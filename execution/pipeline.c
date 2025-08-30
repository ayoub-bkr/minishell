#include "../minishell.h"

void	pipe_ext_handler(t_command *command, t_env *env_vars)
{
	char	**new_envp;
	char	*path;

	if (!command || !command->args || !command->args[0])
	{
		ft_putstr_fd("Invalid command", 1, 2);
		exiting(1);
	}
	if (ft_strchr(command->args[0], '/'))
		path = command->args[0];
	else
		path = get_path(env_vars, command->args[0]);
	if (!path)
	{
		ft_putstrs_fd((char *[]){command->args[0],
			": command not found", 0}, 1, 2);
		exiting(1);
	}
	new_envp = env_filling(env_vars);
	if (execve(path, command->args, new_envp) == -1)
	{
		ft_putstrs_fd((char *[]){command->args[0],
			": command not found", 0}, 1, 2);
		exiting(1);
	}
}

void	pipe_waiting(pid_t pid)
{
	int		w_status;
	pid_t	w_pid;

	w_pid = 0;
	while (w_pid != -1)
	{
		w_pid = wait(&w_status);
		if (w_pid == pid)
		{
			if (WIFEXITED(w_status))
				g_exit_status = WEXITSTATUS(w_status);
			else
				g_exit_status = 1;
		}
	}
}

void	pipe_setup(t_command *cmd, t_env **env_vars, int *fd, int *prev_fd)
{
	if (*prev_fd != -1)
	{
		dup2(*prev_fd, STDIN_FILENO);
		close(*prev_fd);
	}
	if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	redirecting(cmd->redir);
	if (bi_checker(cmd->args[0]))
	{
		bi_handler(&cmd, env_vars);
		exiting(0);
	}
	else
		pipe_ext_handler(cmd, *env_vars);
}

pid_t	pipe_forking(t_command *cmd, t_env **env_vars, int *fd, int *prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		g_exit_status = 1;
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
		pipe_setup(cmd, env_vars, fd, prev_fd);
	else
	{
		if (*prev_fd != -1)
			close(*prev_fd);
		if (cmd->next)
		{
			close(fd[1]);
			*prev_fd = fd[0];
		}
	}
	return (pid);
}

void	piping(t_command *cmd, t_env **env_vars)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;

	prev_fd = -1;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = pipe_forking(cmd, env_vars, fd, &prev_fd);
		cmd = cmd->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	pipe_waiting(pid);
}

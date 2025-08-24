/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:31:15 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/24 19:31:17 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_ext_handler(t_command *command, t_env *env_vars)
{
	char	**new_envp;
	char	*path;

	path = ft_strjoin("/bin/", command->args[0]);
	new_envp = env_filling(env_vars);
	execve(path, command->args, new_envp);
	exit(1);
}

void	pipe_setup(t_command *cmd, int *fd, int *prev_fd)
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
}

void	pipe_handler(t_command *cmd, t_env **env_vars, int *fd, int *prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		pipe_setup(cmd, fd, prev_fd);
		redirecting(cmd->redir);
		if (bi_checker(cmd->args[0]))
		{
			bi_handler(&cmd, env_vars);
			exit(0);
		}
		else
			pipe_ext_handler(cmd, *env_vars);
	}
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
}

void	piping(t_command *cmd, t_env **env_vars)
{
	int		fd[2];
	int		prev_fd;

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
		pipe_handler(cmd, env_vars, fd, &prev_fd);
		cmd = cmd->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	while (wait(NULL) > 0)
		;
}

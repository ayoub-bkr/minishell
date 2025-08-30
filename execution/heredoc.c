/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:44:25 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:44:26 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_her_doc(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	g_exit_status = 130;
	exiting(g_exit_status);
}

void	heredoc_child(t_redir *redir, int fd[2], int i)
{
	char	*line;

	signal(SIGINT, exit_her_doc);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file))
		{
			free(line);
			break ;
		}
		if (should_expand_heredoc(0, 0, i))
			line = expend_heredoc(line);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	exiting(0);
}

void	heredoc_parent(int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, ctrl_c);
	if (WEXITSTATUS(status))
	{
		g_exit_status = WEXITSTATUS(status);
		if (g_exit_status)
			is_heredoc_sig_int(1, 1);
	}
}

void	heredoc(t_redir *redir, int i)
{
	int		fd[2];
	int		pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exiting(1);
	}
	is_heredoc_sig_int(1, 0);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (!pid)
		heredoc_child(redir, fd, i);
	else
		heredoc_parent(pid);
	close(fd[1]);
	redir->heredoc_fd = fd[0];
}

void	heredoc_init(t_command *command)
{
	t_command	*cmd;
	t_redir		*red;
	int			i;

	i = 0;
	cmd = command;
	while (cmd)
	{
		red = cmd->redir;
		while (red)
		{
			if (red->type == 3)
			{
				heredoc(red, i);
				i++;
			}
			red = red->next;
		}
		cmd = cmd->next;
	}
}

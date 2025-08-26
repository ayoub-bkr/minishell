/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:30:57 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/24 19:31:00 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc(t_redir *redir)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exiting(1);
	}
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
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	redir->heredoc_fd = fd[0];
}

void	heredoc_init(t_command *command)
{
	t_command	*cmd;
	t_redir		*red;

	cmd = command;
	while (cmd)
	{
		red = cmd->redir;
		while (red)
		{
			if (red->type == 3)
				heredoc(red);
			red = red->next;
		}
		cmd = cmd->next;
	}
}

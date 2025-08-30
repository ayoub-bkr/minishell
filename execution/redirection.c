/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <aboukent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:44:33 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 16:01:34 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	red_input(t_redir *redir)
{
	int	fd;

	if (!redir->file)
		exiting(1);
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->file);
		exiting(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		exiting(1);
	}
	close(fd);
}

void	red_output(t_redir *redir)
{
	int	fd;

	if (!redir->file)
		exiting(1);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		exiting(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		exiting(1);
	}
	close(fd);
}

void	red_append(t_redir *redir)
{
	int	fd;

	if (!redir->file)
		exiting(1);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		exiting(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		exiting(1);
	}
	close(fd);
}

void	redirecting(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == 0)
			red_input(redir);
		else if (redir->type == 1)
			red_output(redir);
		else if (redir->type == 2)
			red_append(redir);
		else if (redir->type == 3)
		{
			if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
			{
				close(redir->heredoc_fd);
				perror("dup2");
				exiting(1);
			}
		}
		redir = redir->next;
	}
}

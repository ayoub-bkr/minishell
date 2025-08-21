#include "minishell.h"

void	red_input(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	red_output(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	red_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void red_heredoc(t_redir *redir)
{
	int fd[2];
	pipe(fd);

	while (1)
	{
		char *line;

		line = readline("> ");
		if (ft_strcmp(line, redir->file))
		{
			write(fd[1], line, ft_strlen(line));
			break;
		}
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
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
			red_heredoc(redir);
		redir = redir->next;
	}
}

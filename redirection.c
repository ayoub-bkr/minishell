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

void red_heredoc(t_redir *redir, int last_red)
{
	int 	fd[2];

	pipe(fd);
	char *line;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file))
		{
			free(line);
			break;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	if (last_red)
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

void	redirecting(t_redir *redir)
{
	t_redir	*tmp;
	int		last_red;

	tmp = redir;
	while (tmp)
	{
		if (tmp->type == 0)
			red_input(tmp);
		else if (tmp->type == 1)
			red_output(tmp);
		else if (tmp->type == 2)
			red_append(tmp);
		else if (tmp->type == 3)
		{
			last_red = (!tmp->next || tmp->next->type != 3);
			red_heredoc(tmp, last_red);
		}
		tmp = tmp->next;
	}
}

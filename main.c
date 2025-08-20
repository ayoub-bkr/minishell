#include "minishell.h"

char	**env_filling(t_env *head)
{
	int		i;
	t_env	*tmp;
	char	**m;

	i = 0;
	tmp = head;
	m = malloc(sizeof(char *) * (env_lstcount(head) + 1));
	while (tmp)
	{
		m[i++] = tmp->var;
		tmp = tmp->next;
	}
	m[i] = NULL;
	return (m);
}

void	ext_handler(t_command *command, t_env *env_vars)
{
	pid_t	pid;
	char	**new_envp;
	char	*path;

	pid	= fork();
	if (!pid)
	{
		path = ft_strjoin("/bin/", command->args[0]);
		new_envp = env_filling(env_vars);
		execve(path, command->args, new_envp);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	pipe_ext_handler(t_command *command, t_env *env_vars)
{
	char	**new_envp;
	char	*path;

		path = ft_strjoin("/bin/", command->args[0]);
		new_envp = env_filling(env_vars);
		execve(path, command->args, new_envp);
		exit(1);
}

void redirecting(t_redir *redir)
{
    int fd;

    while (redir)
    {
        if (redir->type == 0) // <
        {
            fd = open(redir->file, O_RDONLY);
            if (fd < 0)
            {
                perror(redir->file);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == 1) // >
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == 2) // >>
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
}

void	piping(t_command *command, t_env **env_vars)
{
	int		fd[2];
	pid_t	pid;
	int		prev_fd;

	prev_fd = -1;
	while (command)
	{
		if (command->next)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (command->next)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			redirecting(command->redir);
			if (bi_checker(command->args[0]))
			{
				bi_handler(&command, env_vars);
				exit(0);
			}
			else
				pipe_ext_handler(command, *env_vars);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (command->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		command = command->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	// printf("DEBUG: About to wait for children\n");
	while (wait(NULL) > 0);
		// printf("DEBUG: Child process finished\n");
	// printf("DEBUG: All children finished, returning from piping\n");
}

void	ctrl_c(int s)
{
	(void)s;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	cmd_freeing(t_command **command)
{
	t_command	*tmp;
	int			i;

	while (*command)
	{
		tmp = (*command)->next;
		i = 0;
		while ((*command)->args[i])
			free((*command)->args[i++]);
		free((*command)->args);
		free(*command);
		*command = tmp;
	}
	*command = NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_env		*env_vars;
	t_command	*command;

	if (ac != 1 && av[1])
		return (0);
	env_vars = NULL;
	command = NULL;
	while(*envp)
		env_lstaddback(&env_vars, *envp++);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		init(&command, &env_vars);
		if (!command)
			continue;
		if (command->next)
			piping(command, &env_vars);
		else
		{
			if (bi_checker(command->args[0]))
				bi_handler(&command, &env_vars);
			else if (!access(ft_strjoin("/bin/", command->args[0]), F_OK))
				ext_handler(command, env_vars);
		}
		cmd_freeing(&command);
	}
}
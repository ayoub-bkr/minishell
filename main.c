#include "minishell.h"

char	**env_filling(t_env *head)
{
	int		i;
	t_env	*tmp;
	char	**m;

	i = 0;
	tmp = head;
	m = malloc(sizeof(char *) * (ft_lstcount(head) + 1));
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

void	ft_lstfree(t_env **head)
{
	t_env	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

char	*ft_lstgetvar(t_env *command, char *str)
{
	int	i;

	while (command)
	{
		i = 0;
		while (command->var[i] == str[i] && str[i])
			i++;
		if (command->var[i] == '=')
			return (&command->var[i + 1]);
		command = command->next;
	}
	return ("");
}

char	*replace_variable(t_env *env_vars, char *input)
{
	int	i;
	int	j;
	char	*var;
	char	*new;

	new = "";
	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	j = 1;
	if (input[i] == '$')
	{
		while (input[i + j] != ' ' && input[i + j])
			j++;
		var = ft_lstgetvar(env_vars, ft_substr(input, i + 1, j));
		new = ft_strjoin(ft_substr(input, 0, i), var);
		i += j;
		j = i;
		while (input[j])
			j++;
		new = ft_strjoin(new, ft_substr(input, i, j));
	}
	return (new);
}

void	cmd_lstaddback(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!new)
		return ;
	new->pipe_out = 0;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		tmp->pipe_out = 1;
	}
}

void	init(t_command **command, t_env **env_vars)
{
	char	*input;
	char	**cmds;
	int		i;
	input = readline("minishell$ ");
	if (!input)
	{
		ft_lstfree(env_vars);
		rl_clear_history();
		write(1, "exit\n", 5);
		exit(0);
	}
	else
		add_history(input);
	if (ft_strchr(input, '$'))
		input = replace_variable(*env_vars, input);
	cmds = ft_split(input, '|');
	i = 0;
	while (cmds[i])
	{
		t_command	*new;
	
		new = malloc(sizeof(t_command));
		if (!new)
			exit(1);
		new->args = ft_split(cmds[i], ' ');
		new->next = NULL;
		cmd_lstaddback(command, new);
		i++;
	}
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
		ft_lstaddback(&env_vars, *envp++);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		init(&command, &env_vars);
		if (command->next)
		{
			// printf("DEBUG: Calling piping function\n");
			piping(command, &env_vars);
			// printf("DEBUG: Returned from piping function\n");
		}
		else
		{
			// printf("DEBUG: No piping needed\n");
			if (bi_checker(command->args[0]))
				bi_handler(&command, &env_vars);
			else if (!access(ft_strjoin("/bin/", command->args[0]), F_OK))
				ext_handler(command, env_vars);
		}
		cmd_freeing(&command);
		// printf("DEBUG: About to loop again\n");
	}
}
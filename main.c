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

void	init(t_command **command, t_env **env_vars)
{
	char	*input;

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
	(*command)->args = ft_split(input, ' ');
}

int	main(int ac, char **av, char **envp)
{
	t_env		*env_vars;
	t_command	*command;

	if (ac != 1 && av[1])
		return (0);
	env_vars = NULL;
	command = malloc(sizeof(t_command));
	while(*envp)
		ft_lstaddback(&env_vars, *envp++);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		init(&command, &env_vars);
		
		if (bi_checker(command->args[0]))
			bi_handler(&command, &env_vars);
		else if (!access(ft_strjoin("/bin/", command->args[0]), F_OK))
			ext_handler(command, env_vars);
	}
}
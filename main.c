#include "minishell.h"

int	exp_equal(char *input)
{
	while (*input)
	{
		if (*input == '=')
			return (1);
		input++;
	}
	return (0);
}

int	exp_already(char *input, t_data *cmds)
{
	int		i;
	t_data	*tmp;

	tmp = cmds;
	while (tmp)
	{
		i = 0;
		while (input[i] && input[i] != '=')
		{
			if (input[i] != tmp->varenv[i])
				break ;
			i++;
		}
		if (input[i] == '=' && tmp->varenv[i] == '=')
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	ctrl_c(int s)
{
	(void)s;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_lstfree(t_data **head)
{
	t_data	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

char	*ft_lstgetvar(t_data *cmds, char *var)
{
	int	i;

	while (cmds)
	{
		i = 0;
		while (cmds->varenv[i] == var[i] && var[i])
			i++;
		if (cmds->varenv[i] == '=')
			return (&cmds->varenv[i + 1]);
		cmds = cmds->next;
	}
	return ("");
}

char	*replace_variable(t_data *cmds, char *input)
{
	int	i;
	int	j;
	char	*var;
	char	*new;

	new = malloc()
	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	j = 1;
	if (input[i] == '$')
	{
		while (input[i + j] != ' ' && input[i + j])
			j++;
		var = ft_lstgetvar(cmds, ft_substr(input, i + 1, j));
	}
	return (var);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_data	*cmds = NULL;
	char	cwd_buf[1024];

	if (ac != 1 && av[1])
		return (0);
	while(*envp)
		ft_lstaddback(&cmds, *envp++);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell$ ");
		if (!input)
		{
			ft_lstfree(&cmds);
			rl_clear_history();
			write(1, "exit\n", 5);
			break ;
		}
		else
			add_history(input);
		if (ft_strchr(input, '$'))
			input = replace_variable(cmds, input);
		if (ft_strcmp(input, "env"))
			cmd_env(cmds);
		else if (ft_strncmp(input, "unset ", 6))
			ft_lstremove(&cmds, &input[6]);
		else if (ft_strncmp(input, "export ", 7) && exp_equal(&input[7]) && !exp_already(&input[7], cmds))
			ft_lstaddback(&cmds, &input[7]);
		else if (ft_strncmp(input, "echo -n ", 8))
			ft_putstr(&input[8]);
		else if (ft_strncmp(input, "echo ", 5))
			ft_putstr_nl(&input[5]);
		else if (ft_strncmp(input, "pwd", 4))
			ft_putstr_nl(getcwd(cwd_buf, sizeof(cwd_buf)));
		else if (ft_strncmp(input, "cd ", 3))
			chdir(&input[3]);
		else if (ft_strcmp(input, "exit"))
		{
			write(1, "exit\n", 5);
			break ;
		}
	}
}
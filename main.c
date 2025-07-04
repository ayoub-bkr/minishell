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

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_data	*cmds;

	cmds = malloc(sizeof(t_data));
	while(*envp)
		ft_lstaddback(&cmds, *envp++);
	while (1)
	{
		input = readline("minishell$");
		if (!input)
			break ;
		if (ft_strncmp(input, "env", 3))
			cmd_env(cmds);
		else if (ft_strncmp(input, "echo -n ", 8))
			ft_putstr(&input[8]);
		else if (ft_strncmp(input, "echo ", 5))
		{
			ft_putstr(&input[5]);
			write(1, "\n", 1);
		}
		else if (ft_strncmp(input, "export ", 7) && exp_equal(&input[7]) && !exp_already(&input[7], cmds))
			ft_lstaddback(&cmds, &input[7]);
	}
}
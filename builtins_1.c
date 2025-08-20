#include "minishell.h"

int	exp_equal(char *input)
{
	if (!input)
		return (0);
	while (*input)
	{
		if (*input == '=')
			return (1);
		input++;
	}
	return (0);
}

int	exp_already(char *input, t_env *command)
{
	int		i;
	t_env	*tmp;

	if (!input)
		return (1);
	tmp = command;
	while (tmp)
	{
		i = 0;
		while (input[i] && input[i] != '=')
		{
			if (input[i] != tmp->var[i])
				break ;
			i++;
		}
		if (input[i] == '=' && tmp->var[i] == '=')
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	exp_handler(t_command **command, t_env **env_vars)
{
	char	*tmp;

	if (!exp_equal((*command)->args[1])
		|| exp_already((*command)->args[1], *env_vars))
		return ;
	tmp = ft_strdup((*command)->args[1]);
	if (tmp)
		env_lstaddback(env_vars, tmp);
}

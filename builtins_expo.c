#include "minishell.h"

int	expo_valid_id(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '=')
	{
		if (ft_isalpha(input[i]) || input[i] == '_'
			|| (ft_isdigit(input[i]) && i != 0))
			i++;
		else
			return (0);
	}
	return (1);
}

int	expo_equal(char *input)
{
	if (!input)
		return (0);
	if (*input == '=')
		return (0);
	while (*input)
	{
		if (*input == '=')
			return (1);
		input++;
	}
	return (0);
}

int	expo_already(char *input, t_env *command)
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

void	expo_lstedit(char *input, t_env **env_vars)
{
	int		i;
	char	*to_free;
	t_env	*tmp;

	tmp = *env_vars;
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
		{
			to_free = tmp->var;
			tmp->var = ft_strdup(input);
			free(to_free);
			return ;
		}
		tmp = tmp->next;
	}
}

void	expo_handler(t_command **command, t_env **env_vars)
{
	char	*tmp;

	if ((*command)->args[0] && !(*command)->args[1])
		env_printing(*env_vars, 1);
	else if (!(*command)->args[1]
		|| !expo_equal((*command)->args[1])
		|| !expo_valid_id((*command)->args[1]))
		return ;
	else if (expo_already((*command)->args[1], *env_vars))
		expo_lstedit((*command)->args[1], env_vars);
	else
	{
		tmp = ft_strdup((*command)->args[1]);
		if (tmp)
			env_lstaddback(env_vars, tmp);
	}
}

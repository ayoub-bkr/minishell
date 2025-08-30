#include "../minishell.h"

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
			tmp->var = ft_strdup(input);
			tmp->key = get_key(input);
			tmp->value = get_value(input);
			return ;
		}
		tmp = tmp->next;
	}
}

void	expo_env_init(t_command **command, t_env **env_vars, int i)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (new_var)
	{
		new_var->var = ft_strdup((*command)->args[i]);
		new_var->key = get_key((*command)->args[i]);
		new_var->value = get_value((*command)->args[i]);
		new_var->next = NULL;
		*env_vars = new_var;
	}
}

void	expo_handler(t_command **command, t_env **env_vars)
{
	int	i;

	if ((*command)->args[0] && !(*command)->args[1])
		env_printing(*env_vars, 1);
	i = 1;
	while ((*command)->args[i])
	{
		if (!expo_valid_id((*command)->args[i]))
		{
			ft_putstrs_fd((char *[]){"'", (*command)->args[i],
				"'", ": not a valid identifier", 0}, 1, 2);
			g_exit_status = 1;
			i++;
			continue ;
		}
		else if (expo_already((*command)->args[i], *env_vars))
			expo_lstedit((*command)->args[i], env_vars);
		else if (!*env_vars)
			expo_env_init(command, env_vars, i);
		else
			env_lstaddback(env_vars, (*command)->args[i]);
		i++;
	}
}

#include "minishell.h"

void	ft_putstr(char *str, int nl)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	if (nl)
		write(1, "\n", 1);
}

void	ft_putstrs(char **strs, int nl)
{
	int	i;
	int	j;

	j = 0;
	while (strs[j])
	{
		i = 0;
		while (strs[j][i])
			write(1, &strs[j][i++], 1);
		j++;
	}
	if (nl)
		write(1, "\n", 1);
}

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

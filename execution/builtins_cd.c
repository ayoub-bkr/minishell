/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:42:47 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:42:48 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*env(int set, t_env *value)
{
	static t_env	*val;

	if (set)
		val = value;
	return (val);
}

bool	is_heredoc_sig_int(int set, bool val)
{
	static bool	value;

	if (set)
		value = val;
	return (value);
}

int	expo_valid_id(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '=')
		return (0);
	while (input[i] && input[i] != '=')
	{
		if (ft_isalpha(input[i]) || input[i] == '_'
			|| ((input[i] >= '0' && input[i] <= '9') && i != 0))
			i++;
		else
			return (0);
	}
	return (1);
}

char	*bi_cd_home(t_env *env_vars, char *str)
{
	int		i;
	t_env	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		i = 0;
		while (str[i] && str[i] != '=')
		{
			if (str[i] != tmp->var[i])
				break ;
			i++;
		}
		if (tmp->var[i] == '=')
			return (&tmp->var[i + 1]);
		tmp = tmp->next;
	}
	return (NULL);
}

void	bi_cd(char **args, t_env *env_vars)
{
	char	*dir;
	char	**strs;

	if (!args[1])
	{
		dir = bi_cd_home(env_vars, "HOME");
		if (!dir)
		{
			ft_putstr_fd("cd: HOME not set\n", 0, 2);
			g_exit_status = 1;
			return ;
		}
	}
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		strs = (char *[]){"cd: ", dir, ": ", strerror(errno), 0};
		ft_putstrs_fd(strs, 1, 2);
		g_exit_status = 1;
		return ;
	}
	g_exit_status = 0;
}

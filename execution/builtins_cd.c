/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:34:14 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/25 10:34:15 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	if (!args[1])
	{
		dir = bi_cd_home(env_vars, "HOME");
		if (!dir)
		{
			fprintf(stderr, "cd: HOME not set\n");
			g_exit_status = 1;
			return ;
		}
	}
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		fprintf(stderr, "cd: %s: %s\n", dir, strerror(errno));
		g_exit_status = 1;
		return ;
	}
	g_exit_status = 0;
}

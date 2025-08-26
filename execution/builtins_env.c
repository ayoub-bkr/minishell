/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:30:10 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/24 19:30:16 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_printing(t_env *env_vars, int expo)
{
	t_env	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		if (expo_equal(tmp->var))
		{
			if (expo)
				ft_putstr("declare -x ", 0);
			ft_putstr(tmp->var, 1);
		}
		tmp = tmp->next;
	}
}

void	env_lstremove(t_env **head, char **cmds)
{
	t_env	*tmp;
	t_env	*to_free;

	while (*cmds)
	{
		tmp = *head;
		if (ft_strcmp(tmp->var, *cmds))
		{
			to_free = tmp;
			*head = tmp->next;
			free(to_free);
		}
		while (tmp->next)
		{
			if (ft_strcmp(tmp->next->var, *cmds) == 2)
			{
				to_free = tmp->next;
				tmp->next = tmp->next->next;
				free(to_free);
			}
			else
				tmp = tmp->next;
		}
		cmds++;
	}
}

void	env_lstaddback(t_env **head, char *str)
{
	t_env	*tmp;
	t_env	*m;

	m = gc_calloc(sizeof(t_env));
	if (!m)
		return ;
	if (!*head || !(*head)->var)
		*head = m;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = m;
	}
	m->var = ft_strdup(str);
	m->next = NULL;
}

int	env_lstcount(t_env *head)
{
	t_env	*tmp;
	int		i;

	tmp = head;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**env_filling(t_env *head)
{
	int		i;
	t_env	*tmp;
	char	**m;

	i = 0;
	tmp = head;
	m = gc_calloc(sizeof(char *) * (env_lstcount(head) + 1));
	if (!m)
		return (NULL);
	while (tmp)
	{
		m[i++] = ft_strdup(tmp->var);
		tmp = tmp->next;
	}
	m[i] = NULL;
	return (m);
}

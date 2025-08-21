#include "minishell.h"

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

	m = malloc(sizeof(t_env));
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
	m->var = str;
	m->next = NULL;
}

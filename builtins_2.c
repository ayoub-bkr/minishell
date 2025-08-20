#include "minishell.h"

void	env_printing(t_env *env_vars)
{
	t_env	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		if (exp_equal(tmp->var))
			ft_putstr(tmp->var, 1);
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

void	env_lstremove(t_env **head, char *str)
{
	t_env	*tmp;
	t_env	*freeing;

	tmp = *head;
	if (ft_strcmp(tmp->var, str))
	{
		freeing = tmp;
		*head = tmp->next;
		free(freeing);
		return ;
	}
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->var, str) == 2)
		{
			freeing = tmp->next;
			tmp->next = tmp->next->next;
			free(freeing);
			return ;
		}
		else
			tmp = tmp->next;
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

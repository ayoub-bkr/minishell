#include "minishell.h"

// void    echo(char *str)
// {
//     ft_putstr(str);
// }

void    cmd_env(t_data *cmds)
{
	t_data *tmp;

	tmp = cmds;
	while (tmp)
	{
		ft_putstr(tmp->varenv);
        write(1, "\n", 1);
		tmp = tmp->next;
	}
}

void	ft_lstaddback(t_data **head, char *var)
{
	t_data	*tmp;
	t_data	*m;

	m = malloc(sizeof(t_data));
	if (!m)
		return ;
	if (!*head || !(*head)->varenv)
		*head = m;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = m;
	}
	m->varenv = var;
	m->next = NULL;
}

void	ft_lstrmvback(t_data **head)
{
	t_data	*tmp;
	t_data	*prev;

	if (!*head)
		return ;
	tmp = *head;
	while (tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	free(tmp);
	prev->next = NULL;
}

// int main()
// {
// 	t_data *node1, *node2, *node3;
// 	node1 = malloc(sizeof(t_data));
// 	node1->varenv = "first";
// 	node2 = malloc(sizeof(t_data));
// 	node2->varenv = "second";
// 	node3 = malloc(sizeof(t_data));
// 	node3->varenv = "third";
	
// 	node1->next = node2;
// 	node2->next = node3;
// 	node3->next = NULL;
	
// 	ft_lstaddback(&node1, "fourth");
// 	ft_lstrmvback(&node1);
// 	t_data	*tmp;
// 	tmp = node1;
// 	while (tmp)
// 	{
// 		printf("%s\n", tmp->varenv);
// 		tmp = tmp->next;
// 	}
// }
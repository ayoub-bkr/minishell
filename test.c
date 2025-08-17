#include "minishell.h"

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
}

int	ft_lstcount(t_env *head)
{
	t_env *tmp;
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
	m = malloc(sizeof(char *) * (ft_lstcount(head) + 1));
	while (tmp)
	{
		m[i++] = tmp->var;
		tmp = tmp->next;
	}
	m[i] = NULL;
	return (m);
}	

int main()
{
	t_env *node1, *node2, *node3;
	node1 = malloc(sizeof(t_env));
	node1->var = "first";
	node2 = malloc(sizeof(t_env));
	node2->var = "second";
	node3 = malloc(sizeof(t_env));
	node3->var = "third";
	
	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

	char	**str;
	str = env_filling(node1);
	// t_env	*tmp;
	// tmp = node1;
	// while (tmp)
	// {
	// 	printf("%s\n", tmp->var);
	// 	tmp = tmp->next;
	// }
	printf("%s", str[5]);
}
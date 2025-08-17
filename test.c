#include <unistd.h>

void	ft_putstrs(char **strs)
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

int	main(int ac, char **av)
{
	ft_putstrs(&av[1]);
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
// 	ft_lstremove(&node1, "first");
// 	t_data	*tmp;
// 	tmp = node1;
// 	while (tmp)
// 	{
// 		printf("%s\n", tmp->varenv);
// 		tmp = tmp->next;
// 	}
// }
#include "minishell.h"

// void	tmp_parcing(t_line **head, char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (ft_strncmp(str, "echo", 4))
// 		ft_lstaddback(head, )
// }

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_line	*line;

	int i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	while (1)
	{
		input = readline("minishell$");
		if (!input)
			break ;
		if (ft_strcmp(input, "echo -n"))
		{
			
		}
			printf("yes\n");
		else
			printf("no\n");
	}
}
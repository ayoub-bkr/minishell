#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_data	*cmds;

	cmds = malloc(sizeof(t_data));
	while(*envp)
		ft_lstaddback(&cmds, *envp++);
	while (1)
	{
		input = readline("minishell$");
		if (!input)
			break ;
		if (ft_strncmp(input, "env", 3))
			cmd_env(cmds);
		else if (ft_strncmp(input, "echo -n ", 8))
			ft_putstr(&input[8]);
		else if (ft_strncmp(input, "echo ", 5))
		{
			ft_putstr(&input[5]);
			write(1, "\n", 1);
		}
		else if (ft_strncmp(input, "export ", 7))
			ft_lstaddback(&cmds, &input[7]);
	}
}
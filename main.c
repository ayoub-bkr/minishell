#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$");
		if (!input)
			break ;
		if (ft_strcmp(input, "echo"))
			printf("%s\n", input);
	}
}
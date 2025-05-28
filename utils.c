#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
    int             i;
	unsigned char	*st1;
	unsigned char	*st2;

	if (!s1 || !s2)
		return (0);
    st1 = (unsigned char *)s1;
	st2 = (unsigned char *)s2;
    i = 0;
	while ((st1[i] || st2[i]))
	{
		if (st1[i] != st2[i])
			return (0);
		i++;
	}
	return (1);
}

// int main(int ac, char **av)
// {
//     printf("%d", ft_strncmp(av[1], av[2], 4));
// }
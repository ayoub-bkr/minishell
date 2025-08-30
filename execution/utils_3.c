#include "../minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

void	ft_putstr(char *str, int nl)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	if (nl)
		write(1, "\n", 1);
}

void	ft_putstr_fd(char *str, int nl, int fd)
{
	int	len;

	if (str == NULL)
		return ;
	len = ft_strlen(str);
	write(fd, str, len);
	if (nl)
		write(fd, "\n", 1);
}

void	ft_putstrs_fd(char *strs[], int nl, int fd)
{
	if (strs == NULL)
		return ;
	while (*strs)
	{
		ft_putstr_fd(*strs, 0, fd);
		strs++;
	}
	if (nl)
		write(fd, "\n", 1);
}

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
		if (strs[j + 1])
			write(1, " ", 1);
		j++;
	}
	if (nl)
		write(1, "\n", 1);
}

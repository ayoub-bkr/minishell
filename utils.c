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
		{
			if (st1[i] == '=' && !st2[i])
				return (2);
			else
				return (0);
		}
		i++;
	}
	return (1);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
    int             i;
	unsigned char	*st1;
	unsigned char	*st2;

	if (!s1 || !s2)
		return (0);
	st1 = (unsigned char *)s1;
	st2 = (unsigned char *)s2;
	i = 0;
	while ((st1[i] || st2[i]) && i < n)
	{
		if (st1[i] != st2[i])
			return (0);
		i++;
	}
	return (1);
}

void	ft_putstr(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
}

void	ft_putstr_nl(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	write(1, "\n", 1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	int		i;
	char	*m;

	i = 0;
	m = malloc(ft_strlen(s) + 1);
	if (!m)
		return (0);
	while (s[i])
	{
		*(m + i) = s[i];
		i++;
	}
	*(m + i) = '\0';
	return (m);
}

int		ft_strchr(char *str, char c)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*m;

	i = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	m = (char *) malloc(len + 1);
	if (!m)
		return (0);
	while (i < len)
	{
		m[i] = s[start + i];
		i++;
	}
	m[i] = '\0';
	return (m);
}

// int main(int ac, char **av)
// {
//     printf("%d", ft_strcmp(av[1], av[2]));
// }
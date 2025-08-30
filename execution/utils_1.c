#include "../minishell.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int				i;
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

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	int		i;
	char	*m;

	if (!s)
		return (NULL);
	i = 0;
	m = gc_calloc(ft_strlen(s) + 1);
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

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

#include "minishell.h"

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

char	*ft_substr(char *s, int start, int len)
{
	int		i;
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

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*m;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	m = (char *)malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!m)
		return (0);
	while (s1[i])
	{
		m[i] = s1[i];
		i++;
	}
	while (s2[j])
		m[i++] = s2[j++];
	m[i] = '\0';
	return (m);
}

static int	words(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**freeing(char **m, int i)
{
	while (i-- > 0)
		free(m[i]);
	free(m);
	return (NULL);
}

static char	*skip(char *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}

char	**ft_split(char *s, char c)
{
	int		i;
	int		len;
	char	**m;

	i = 0;
	if (!s)
		return (NULL);
	m = malloc(sizeof(char *) * (words(s, c) + 1));
	if (!m)
		return (NULL);
	while (*s)
	{
		len = 0;
		s = skip(s, c);
		if (*s == '\0')
			break ;
		while (s[len] != c && s[len])
			len++;
		m[i] = ft_substr(s, 0, len);
		if (m[i++] == NULL)
			return (freeing (m, i));
		s += len;
	}
	m[i] = NULL;
	return (m);
}
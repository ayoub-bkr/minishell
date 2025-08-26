/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:31:43 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/24 19:31:45 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	m = (char *) gc_calloc(len + 1);
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
	m = (char *)gc_calloc (ft_strlen(s1) + ft_strlen(s2) + 1);
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

int	split_words(char *s, char c)
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

char	**split_freeing(char **m, int i)
{
	while (i-- > 0)
		free(m[i]);
	free(m);
	return (NULL);
}

char	**ft_split(char *s, char c)
{
	char	**m;

	int (i), (len);
	i = 0;
	if (!s)
		return (NULL);
	m = gc_calloc(sizeof(char *) * (split_words(s, c) + 1));
	if (!m)
		return (NULL);
	while (*s)
	{
		len = 0;
		while (*s == c)
			s++;
		if (*s == '\0')
			break ;
		while (s[len] != c && s[len])
			len++;
		m[i] = ft_substr(s, 0, len);
		if (m[i++] == NULL)
			return (NULL);
		s += len;
	}
	m[i] = NULL;
	return (m);
}

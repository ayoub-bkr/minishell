#include "../../../minishell.h"

t_string *ft_newstr(char *s)
{
	t_string	*new;
	int			len;

	new = gc_calloc(sizeof(t_string));
	if (!new)
		return (NULL);
	len = strlen(s);
	new->str = gc_calloc(sizeof(char) * (len + 1));
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(new->str, s, len);
	new->str[len] = '\0';
	new->cap = len + 1;
	new->len = len;
	return (new);
}


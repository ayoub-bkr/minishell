#include "../../../minishell.h"

void	*ft_memcpy(void *dest, void *src, int n)
{
	unsigned const char	*s;
	unsigned char		*d;

	d = (unsigned char *)dest;
	s = (unsigned const char *)src;
	while (n--)
		*d++ = *s++;
	return (dest);
}

void	string_append_char(t_string *str, char c)
{
	char	*new_str;
	int		new_cap;

	if (str->len >= str->cap - 1)
	{
		new_cap = str->cap * 2;
		new_str = gc_calloc(new_cap);
		if (!new_str)
			return ;
		ft_memcpy(new_str, str->str, str->len);
		str->str = new_str;
		str->cap = new_cap;
	}
	str->str[str->len++] = c;
	str->str[str->len] = '\0';
}

void	string_append_str(t_string *dest, char *src)
{
	int	i;

	if (!src || !dest)
		return ;
	i = 0;
	while (src[i])
	{
		string_append_char(dest, src[i]);
		i++;
	}
}

t_string	*string_init(int initial_cap)
{
	t_string	*str;

	str = gc_calloc(sizeof(t_string));
	if (!str)
		return (NULL);
	str->str = gc_calloc(initial_cap);
	if (!str->str)
		return (NULL);
	str->len = 0;
	str->cap = initial_cap;
	str->str[0] = '\0';
	return (str);
}

char	*get_env_value(char *var_name, t_env *env_vars)
{
	t_env	*cur;

	if (!var_name || !env_vars)
		return ("");
	cur = env_vars;
	while (cur)
	{
		if (cur->key && ft_strcmp(cur->key, var_name) == 1)
			return (cur->value ? cur->value : "");
		cur = cur->next;
	}
	return ("");
}

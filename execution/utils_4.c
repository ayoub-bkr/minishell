#include "../minishell.h"

void create_key_value(t_env *single_var, t_gc *gc)
{
	single_var->key = get_key(single_var->var, gc);
	single_var->value = get_value(single_var->var, gc);
}

void create_key_value_pairs(t_env *env_vars, t_gc *gc)
{
	t_env *cur = env_vars;
	while (cur)
	{
		create_key_value(cur, gc);
		cur = cur->next;
	}
}

char *get_key(char *s, t_gc *gc)
{
	int i = 0;
	while (s[i] && s[i] != '=')
		i++;
	// char *tmp = malloc(sizeof(char) * (i + 1));
	char *tmp = gc_malloc(gc, (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		tmp[i] = s[i];
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

char *get_value(char *s, t_gc *gc)
{
	int i = 0;
	int start = 0;
	int len = 0;
	while (s[i] && s[i] != '=')
		i++;
	start = ++i;
	while (s[len + start])
		len++;
	// char *tmp = malloc(sizeof(char) * (len + 1));
    char *tmp = gc_malloc(gc, (len + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[start])
		tmp[i++] = s[start++];
	tmp[i] = 0;
	return (tmp);
}

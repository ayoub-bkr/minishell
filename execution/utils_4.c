#include "../minishell.h"

void create_key_value(t_env *single_var)
{
	single_var->key = get_key(single_var->var);
	single_var->value = get_value(single_var->var);
}

void create_key_value_pairs(t_env *env_vars)
{
	t_env *cur = env_vars;
	while (cur)
	{
		create_key_value(cur);
		cur = cur->next;
	}
}

char *get_key(char *s)
{
	int i = 0;
	while (s[i] && s[i] != '=')
		i++;
	// char *tmp = malloc(sizeof(char) * (i + 1));
	char *tmp = malloc(i + 1);
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

char *get_value(char *s)
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
    char *tmp = malloc(len + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[start])
		tmp[i++] = s[start++];
	tmp[i] = 0;
	return (tmp);
}

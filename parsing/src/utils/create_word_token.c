#include "../../../minishell.h"

char *create_quote_word(t_string *str)
{
	int len;
	char *s;

	s = NULL;
	str->peek++;
	str->start = str->peek;
	str->end = str->peek;
	while (str->peek < str->len && str->str[str->peek] != str->quote)
	{
		str->end++;
		str->peek++;
	}
	len = str->end - str->start;
	s = gc_calloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	ft_memcpy(s, str->str + str->start, len);
	s[len] = '\0';
	str->peek++;
	return (s);
}

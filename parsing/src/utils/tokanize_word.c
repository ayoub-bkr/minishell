#include "../../../minishell.h"

char *tokanize_word(t_string *str)
{
	int len;
	char *s;

	s = NULL;
	str->start = str->peek;
	str->end = str->peek;
	while (str->peek < str->len && !is_delimiter(str->str[str->peek]))
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
	return (s);
}

#include "../../../minishell.h"

char	*collect_delimiter(t_string *str)
{
	int len;
	char *s;

	s = NULL;
	str->del = str->str[str->peek];
	str->start = str->peek;
	str->end = str->peek;
	while ((str->peek < str->len) && (str->str[str->peek] == str->del))
	{
		str->peek++;
		str->end++;
	}
	len = str->end - str->start;
	s = gc_calloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	ft_memcpy(s, str->str + str->start, len);
	s[len] = '\0';
	return (s);
}

void handle_delimiter(t_string *str, t_list **head)
{
	char *word;
	char *delimiter;

	word = NULL;
	delimiter = NULL;
	if (str->str[str->peek] == '"' || str->str[str->peek] == '\'')
	{
		str->quote = str->str[str->peek];
		word = create_quote_word(str);
		if (word)
			create_token_node(head, word);
	}
	else
	{
		delimiter = collect_delimiter(str);
		if (delimiter)
			create_token_node(head, delimiter);
	}
}

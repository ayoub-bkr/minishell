#include "../../../minishell.h"

void	add_token(t_list **head, char *str, t_token_type type)
{
	t_list	*node;

	node = gc_calloc(sizeof(t_list));
	node->token = gc_calloc(sizeof(t_token));
	node->token->type = type;
	node->token->str = str;
	node->next = NULL;
	ft_lstadd_back(head, node);
}

char	*extract_word(char **p, int size)
{
	char	*string;

	string = gc_calloc((size + 1) * sizeof(char));
	ft_memcpy(string, *p, size);
	string[size] = '\0';
	*p += size;
	return (string);
}

bool	tokenize_word(char **p, t_list **head)
{
	char	in_quote;
	char	*str;
	int		size;

	str = *p;
	in_quote = '\0';
	while (*str)
	{
		if (!in_quote && (*str == '"' || *str == '\''))
			in_quote = *str;
		else if (*str == in_quote)
			in_quote = '\0';
		else if (!in_quote && strchr("><| \t", *str))
			break ;
		str++;
	}
	size = str - *p;
	if (size <= 0)
		return (false);
	add_token(head, extract_word(p, size), T_WORD);
	return (true);
}

bool	tokenize_metachar(char **str, t_list **head)
{
	if ((*str)[0] == '|')
		return ((*str)++, add_token(head, NULL, T_PIPE), true);
	if ((*str)[0] == '>' && (*str)[1] == '>')
		return (*str += 2, add_token(head, NULL, T_APPEND), true);
	if ((*str)[0] == '<' && (*str)[1] == '<')
		return (*str += 2, add_token(head, NULL, T_HEREDC), true);
	if ((*str)[0] == '>')
		return ((*str)++, add_token(head, NULL, T_RED_OUT), true);
	if ((*str)[0] == '<')
		return ((*str)++, add_token(head, NULL, T_RED_IN), true);
	return (false);
}

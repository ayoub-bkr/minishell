#include "../../../minishell.h"

void create_token_node(t_list **head, char *s)
{
	Token *token;
	t_list *new;
	TokenType t_type;

	t_type = return_token_type(s);
	token = ft_newtoken(s, t_type);
	new = ft_lstnew(token);
	ft_lstadd_back(head, new);
}

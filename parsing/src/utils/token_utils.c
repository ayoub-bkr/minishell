/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:36 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 18:31:37 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_string	*ft_newstr(char *s)
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

t_token	*ft_newtoken(char *s, t_token_type type)
{
	t_token	*new;

	if (!s)
		return (NULL);
	new = gc_calloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = s;
	new->type = type;
	return (new);
}

t_list	*ft_lstnew(t_token *token)
{
	t_list	*new;

	if (!token)
		return (NULL);
	new = gc_calloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->token = token;
	new->next = NULL;
	return (new);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **head, t_list *new)
{
	t_list	*last;

	last = NULL;
	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast(*head);
	last->next = new;
}

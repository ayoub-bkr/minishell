/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 03:34:23 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 03:34:25 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

Token	*ft_newtoken(char *s, TokenType type)
{
	Token	*new;

	if (!s)
		return (NULL);
	new = gc_calloc(sizeof(Token));
	if (!new)
		return (NULL);
	new->str = s;
	new->type = type;
	return (new);
}

t_list	*ft_lstnew(Token *token)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_t_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 03:36:53 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 03:36:54 by mohel-mo         ###   ########.fr       */
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

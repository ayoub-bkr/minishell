/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:58 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 18:31:59 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	expand_token(char *original, t_string *result, t_env *env_vars)
{
	int					i;
	char				current_quote;
	t_expand_context	ctx;

	i = 0;
	current_quote = 0;
	ctx = (t_expand_context){original, &i, current_quote, result, env_vars};
	while (original[i])
	{
		if (original[i] == '\'' && current_quote != '"')
		{
			handle_quote('\'', &current_quote);
			i++;
		}
		else if (original[i] == '"' && current_quote != '\'')
		{
			handle_quote('"', &current_quote);
			i++;
		}
		else if (original[i] == '$' && current_quote != '\'')
			handle_dollar_sign(&ctx);
		else
			string_append_char(result, original[i++]);
	}
}

void	process_token_expansion(t_token *token, t_env *env_vars)
{
	t_string	*result;
	char		*original;

	if (!token || !token->str)
		return ;
	original = token->str;
	result = string_init(ft_strlen(original) * 2);
	if (!result)
		return ;
	expand_token(original, result, env_vars);
	token->str = ft_strdup(result->str);
}

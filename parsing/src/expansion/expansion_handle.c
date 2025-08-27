/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:30:24 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 18:30:25 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	handle_quote(char quote, char *current_quote)
{
	if (!*current_quote)
		*current_quote = quote;
	else if (*current_quote == quote)
		*current_quote = 0;
}

void	handle_exit_status(t_string *result)
{
	char	status_str[12];

	snprintf(status_str, sizeof(status_str), "%d", g_exit_status);
	string_append_str(result, status_str);
}

void	handle_env_var(char *original, int *i, t_string *result,
		t_env *env_vars)
{
	int		start;
	int		len;
	char	*var_name;
	char	*value;

	start = *i + 1;
	(*i)++;
	while (original[*i] && (ft_isalpha(original[*i]) || original[*i] == '_'
			|| (original[*i] >= '0' && original[*i] <= '9')))
		(*i)++;
	len = *i - start;
	var_name = gc_calloc(len + 1);
	if (var_name)
	{
		ft_memcpy(var_name, original + start, len);
		var_name[len] = '\0';
		value = get_env_value(var_name, env_vars);
		if (value)
			string_append_str(result, value);
	}
}

void	handle_dollar_sign(t_expand_context *ctx)
{
	if (ctx->original[*ctx->i + 1] && ctx->original[*ctx->i + 1] == '?')
	{
		handle_exit_status(ctx->result);
		*ctx->i += 2;
	}
	else if (ft_isalpha(ctx->original[*ctx->i + 1])
		|| ctx->original[*ctx->i + 1] == '_')
		handle_env_var(ctx->original, ctx->i, ctx->result, ctx->env_vars);
	else
		string_append_char(ctx->result, ctx->original[(*ctx->i)++]);
}

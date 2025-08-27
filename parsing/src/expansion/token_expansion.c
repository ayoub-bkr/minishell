#include "../../../minishell.h"

void	expand_token(char *original, t_string *result, t_env *env_vars)
{
	int		i;
	char	current_quote;

	i = 0;
	current_quote = 0;
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
			handle_dollar_sign(original, &i, current_quote, result, env_vars);
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

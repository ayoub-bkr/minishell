#include "../../../minishell.h"

bool	has_quote(int set, bool value)
{
	static bool	val;

	if (set)
		val = value;
	return (val);
}

char	*ft_lstgetvar(t_env *command, char *str)
{
	int	i;

	while (command)
	{
		i = 0;
		while (command->var[i] == str[i] && str[i])
			i++;
		if (command->var[i] == '=')
			return (&command->var[i + 1]);
		command = command->next;
	}
	return ("");
}

char	*expend_heredoc(char *line)
{
	int		i;
	int		j;
	char	*var;
	char	*new;

	new = line;
	i = 0;
	while (line[i] && line[i] != '$')
		i++;
	j = 1;
	if (line[i] == '$')
	{
		while (is_valid_var_name_char(line[i + j]) && line[i + j])
			j++;
		var = ft_lstgetvar(env(0, 0), ft_substr(line, i + 1, j));
		new = ft_strjoin(ft_substr(line, 0, i), var);
		i += j;
		j = i;
		while (line[j])
			j++;
		new = ft_strjoin(new, ft_substr(line, i, j));
	}
	return (new);
}

void	expand_token(char *original, t_string *result, t_env *env_vars)
{
	int					i;
	char				cur_quote;
	t_expand_context	ctx;

	i = 0;
	cur_quote = 0;
	ctx = (t_expand_context){original, &i, cur_quote, result, env_vars};
	has_quote(1, 0);
	while (original[i])
	{
		if (original[i] == '\'' && cur_quote != '"')
		{
			handle_quote('\'', &cur_quote);
			i++;
		}
		else if (original[i] == '"' && cur_quote != '\'')
		{
			handle_quote('"', &cur_quote);
			i++;
		}
		else if (original[i] == '$' && cur_quote != '\'' && !is_heredoc(0, 0))
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

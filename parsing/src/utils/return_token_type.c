#include "../../../minishell.h"

TokenType return_token_type(const char *s)
{
	if (!s || !*s)
		return (0);
	if (s[0] == '|')
		return (T_PIPE);
	else if (s[0] == '>')
	{
		if (s[1] != '\0' && s[1] == '>')
			return (T_APPEND);
		else
			return (T_RED_OUT);
	}
	else if (s[0] == '<')
	{
		if (s[1] != '\0' && s[1] == '<')
			return (T_HEREDOC);
		else
			return (T_RED_IN);
	}
	return (0);
}

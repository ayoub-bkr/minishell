#include "../minishell.h"

bool	is_heredoc(t_token_type type, int set)
{
	static bool	val;

	if (set == 1)
		val = (type == T_HEREDC);
	return (val);
}

bool	should_expand_heredoc(int set, bool value, int i)
{
	static bool	val[16];

	if (set)
		val[i] = value;
	if (set == -1)
	{
		i = 0;
		while (i < 16)
		{
			val[i] = true;
			i++;
		}
	}
	return (val[i]);
}

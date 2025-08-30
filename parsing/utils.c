#include "../minishell.h"

int	is_alpha(char ch)
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int	is_digit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

int	is_valid_var_name_char(char ch)
{
	if (!is_alpha(ch) && !is_digit(ch) && ch != '_' && ch != '?')
		return (0);
	return (1);
}

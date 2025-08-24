#include "../../../minishell.h"

int is_delimiter(char c)
{
	if (c == '"' || c == ' ' || c == '|' || c == '>' || c == '<' || c == '\'')
		return (1);
	return (0);
}

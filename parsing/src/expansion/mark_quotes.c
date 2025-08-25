#include "../../../minishell.h"

void	mark_helper(char *c, quote_state *state)
{
        if (*c == '\'' && *state == UNQUOTED)
        {
            *c = -1;
            *state = IN_SINGLE;
        }
        else if (*c == '\'' && *state == IN_SINGLE)
        {
            *c = -1;
            *state = UNQUOTED;
        }
        else if (*c == '"' && *state == UNQUOTED)
        {
            *c = -2;
            *state = IN_DOUBLE;
        }
        else if (*c == '"' && *state == IN_DOUBLE)
        {
            *c = -2;
            *state = UNQUOTED;
        }
}

void	mark_quotes(char *str)
{
    int			i;
    quote_state	state;

	state = UNQUOTED;
	i = 0;
    while (str[i])
    {
		mark_helper(&str[i], &state);
        i++;
    }
}

void	remove_quote_markers(t_string *input, t_string *output)
{
    int	i;

	i = 0;
    if (!input || !output)
        return;
    while (i < input->len)
    {
        if (input->str[i] != -1 && input->str[i] != -2)
            string_append_char(output, input->str[i]);
        i++;
    }
}

#include "../../../minishell.h"

int needs_expansion(char *str)
{
    if (!str)
        return 0;
    int i = 0;
    char in_single_quote = 0;
    while (str[i])
    {
        if (str[i] == -1)
            in_single_quote = !in_single_quote;
        else if (str[i] == '$' && !in_single_quote)
            return 1;
        i++;
    }
    return 0;
}

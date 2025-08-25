#include "../../../minishell.h"

int quote_syntax_error(t_list *head)
{
    if (!head)
        return (0);
    
    t_list *cur = head;
    while (cur)
    {
        if (cur->token->type == T_WORD)
        {
            char *str = cur->token->str;
            int i = 0;
            char in_quote = 0;  // 0=none, '"'=double, '\''=single
            
            while (str[i])
            {
                if ((str[i] == '"' || str[i] == '\'') && !in_quote)
                    in_quote = str[i];  // Start quote
                else if (str[i] == in_quote)
                    in_quote = 0;       // End quote
                i++;
            }
            
            if (in_quote)  // Quote was opened but never closed
            {
                printf("[+] syntax error\n");
                printf("{-} unclosed quote\n");
                return (0);
            }
        }
        cur = cur->next;
    }
    return (1);
}

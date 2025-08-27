#include "../../../minishell.h"

void process_token_expansion(Token *token, t_env *env_vars)
{
    t_string *result;
    char *original;
    int i;
    char current_quote;

    if (!token || !token->str)
        return;

    original = token->str;
    result = string_init(ft_strlen(original) * 2);
    if (!result)
        return;

    i = 0;
    current_quote = 0;
    while (original[i])
    {
        if (original[i] == '\'' && current_quote != '"')
        {
            if (!current_quote)
                current_quote = '\'';
            else if (current_quote == '\'')
                current_quote = 0;
            i++;
            continue;
        }
        else if (original[i] == '"' && current_quote != '\'')
        {
            if (!current_quote)
                current_quote = '"';
            else if (current_quote == '"')
                current_quote = 0;
            i++;
            continue;
        }
        else if (original[i] == '$' && current_quote != '\'')
        {
            if (original[i + 1] && original[i + 1] == '?')
            {
                char status_str[12];
                snprintf(status_str, sizeof(status_str), "%d", g_exit_status);
                string_append_str(result, status_str);
                i += 2;
                continue;
            }
            else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_')
            {
                int start = i + 1;
                i++;
                while (original[i] && (ft_isalpha(original[i]) || original[i] == '_' || 
                       (original[i] >= '0' && original[i] <= '9')))
                    i++;
                int len = i - start;
                char *var_name = gc_calloc(len + 1);
                if (var_name)
                {
                    ft_memcpy(var_name, original + start, len);
                    var_name[len] = '\0';
                    char *value = get_env_value(var_name, env_vars);
                    if (value)
                        string_append_str(result, value);
                }
                continue;
            }
            string_append_char(result, original[i]);
        }
        else
            string_append_char(result, original[i]);
        i++;
    }
    token->str = ft_strdup(result->str);
}

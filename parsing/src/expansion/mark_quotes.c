#include "../../../minishell.h"

static void string_append_char(t_string *str, char c)
{
    char *new_str;
    int new_cap;

    if (str->len >= str->cap - 1)
    {
        new_cap = str->cap * 2;
        new_str = gc_calloc(new_cap);
        if (!new_str)
            return;
        ft_memcpy(new_str, str->str, str->len);
        // free(str->str);
        str->str = new_str;
        str->cap = new_cap;
    }
    str->str[str->len++] = c;
    str->str[str->len] = '\0';
}

static void string_append_str(t_string *dest, char *src)
{
    int i;

    if (!src || !dest)
        return;
    i = 0;
    while (src[i])
    {
        string_append_char(dest, src[i]);
        i++;
    }
}

static t_string *string_init(int initial_cap)
{
    t_string *str;

    str = gc_calloc(sizeof(t_string));
    if (!str)
        return NULL;
    str->str = gc_calloc(initial_cap);
    if (!str->str)
        return NULL;
    str->len = 0;
    str->cap = initial_cap;
    str->str[0] = '\0';
    return str;
}

static char *get_env_value(char *var_name, t_env *env_vars)
{
    t_env *cur;

    if (!var_name || !env_vars)
        return "";
    
    cur = env_vars;
    while (cur)
    {
        if (cur->key && ft_strcmp(cur->key, var_name) == 1)
            return cur->value ? cur->value : "";
        cur = cur->next;
    }
    return "";
}

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

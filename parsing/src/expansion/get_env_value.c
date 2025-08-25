#include "../../../minishell.h"

int is_valid_var_name_char(char c)
{
    return (ft_isalpha(c) || ft_isdigit(c) || c == '_');
}

char *get_env_value(char *var_name, t_env *env_vars)
{
    if (!var_name || !env_vars)
        return "";
    t_env *cur = env_vars;
    while (cur)
    {
        if (cur->key && ft_strcmp(cur->key, var_name))
            return cur->value ? cur->value : "";
        cur = cur->next;
    }
    return "";
}

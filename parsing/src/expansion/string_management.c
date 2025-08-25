#include "../../../minishell.h"

t_string *string_init(int initial_cap)
{
    t_string *str = malloc(sizeof(t_string));
    if (!str)
        return NULL;
    str->str = malloc(initial_cap);
    if (!str->str)
    {
        free(str);
        return NULL;
    }
    str->len = 0;
    str->cap = initial_cap;
    str->str[0] = '\0';
    return str;
}

void string_free(t_string *str)
{
    if (str)
    {
        if (str->str)
            free(str->str);
        free(str);
    }
}

void string_append_char(t_string *str, char c)
{
    if (str->len >= str->cap - 1)
    {
        int new_cap = str->cap * 2;
        char *new_str = malloc(new_cap);
        if (!new_str)
            return ;
        ft_memcpy(new_str, str->str, str->len);
        free(str->str);
        str->str = new_str;
        str->cap = new_cap;
    }
    str->str[str->len++] = c;
    str->str[str->len] = '\0';
}

void string_append_str(t_string *dest, char *src)
{
    if (!src)
        return;
    int i = 0;
    while (src[i])
        string_append_char(dest, src[i++]);
}

#include "../../../minishell.h"

void expand_variable_at_position(char *str, int *i, t_string *result, t_env *env_vars)
{
    (*i)++; // Skip the $
    int start = *i;
    
    // Extract variable name (alphanumeric + underscore)
    while (str[*i] && is_valid_var_name_char(str[*i]))
        (*i)++;
    
    if (*i == start) // No valid variable name after $
    {
        string_append_char(result, '$');  // Keep literal $
        (*i)--; // Adjust for main loop increment
        return;
    }
    
    // Create variable name string
    int var_len = *i - start;
    char *var_name = malloc(var_len + 1);
    if (!var_name)
    {
        (*i)--; // Adjust for main loop increment
        return;
    }
    
    ft_memcpy(var_name, &str[start], var_len);
    var_name[var_len] = '\0';
    
    // Get variable value and append to result
    char *value = get_env_value(var_name, env_vars);
    string_append_str(result, value);
    
    free(var_name);
    (*i)--; // Adjust for main loop increment
}

void expand_variables(char *str, t_string *result, t_env *env_vars)
{
    if (!str || !result)
        return;
        
    char current_quote = 0;  // 0=unquoted, -1=single, -2=double
    
    int i = 0;
    while (str[i])
    {
        // Handle quote state changes
        if (str[i] == -1 || str[i] == -2)
        {
            if (current_quote == 0)
                current_quote = str[i];      // Enter quote mode
            else if (current_quote == str[i])
                current_quote = 0;           // Exit quote mode
            // Don't append quote markers to result
        }
        // Handle variable expansion (only outside single quotes)
        else if (str[i] == '$' && current_quote != -1)
        {
            expand_variable_at_position(str, &i, result, env_vars);
        }
        // Handle regular characters
        else
        {
            string_append_char(result, str[i]);
        }
        i++;
    }
}

#include "../../../minishell.h"

void mark_quotes(char *str)
{
    int i = 0;
    quote_state state = UNQUOTED;
    
    while (str[i])
    {
        if (str[i] == '\'' && state == UNQUOTED)
        {
            str[i] = -1;
            state = IN_SINGLE;
        }
        else if (str[i] == '\'' && state == IN_SINGLE)
        {
            str[i] = -1;
            state = UNQUOTED;
        }
        else if (str[i] == '"' && state == UNQUOTED)
        {
            str[i] = -2;
            state = IN_DOUBLE;
        }
        else if (str[i] == '"' && state == IN_DOUBLE)
        {
            str[i] = -2;
            state = UNQUOTED;
        }
        // Inner quotes become regular characters
        i++;
    }
}


/*
 * CONTROL FLOW:
 * 1. process_token_expansion() - Main entry point
 * 2. mark_quotes() - Replace quotes with markers
 * 3. needs_expansion() - Check if $ variables exist
 * 4. expand_variables() - Process variables and build result
 * 5. remove_quote_markers() - Clean up markers from result
 * 6. Replace original token->str with final result
 */

// ============================================================================
// DYNAMIC STRING MANAGEMENT (without realloc)
// ============================================================================

t_string *string_init(int initial_cap)
{
    t_string *str = gc_calloc(sizeof(t_string));
    if (!str)
        return NULL;
    str->str = gc_calloc(initial_cap);
    if (!str->str)
    {
        // free(str);
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
        // Double the capacity
        int new_cap = str->cap * 2;
        char *new_str = gc_calloc(new_cap);
        if (!new_str)
            return; // Handle gc_calloc failure
        
        // Copy old data
        ft_memcpy(new_str, str->str, str->len);
        
        // Free old buffer and update
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

// ============================================================================
// STEP 1: MARK QUOTES (modifies string in-place)
// ============================================================================
/*
void mark_quotes(char *str)
{
    if (!str)
        return;
        
    int i = 0;
    char current_quote = 0;  // Track active quote type
    
    while (str[i])
    {
        if (str[i] == '\'' && current_quote == 0)
        {
            str[i] = -1;           // Mark opening single quote
            current_quote = '\'';  // Enter single quote mode
        }
        else if (str[i] == '\'' && current_quote == '\'')
        {
            str[i] = -1;           // Mark closing single quote
            current_quote = 0;     // Exit quote mode
        }
        else if (str[i] == '"' && current_quote == 0)
        {
            str[i] = -2;           // Mark opening double quote
            current_quote = '"';   // Enter double quote mode
        }
        else if (str[i] == '"' && current_quote == '"')
        {
            str[i] = -2;           // Mark closing double quote
            current_quote = 0;     // Exit quote mode
        }
        i++;
    }
}
*/

// ============================================================================
// STEP 2: CHECK IF EXPANSION NEEDED
// ============================================================================

int needs_expansion(char *str)
{
    if (!str)
        return 0;
        
    int i = 0;
    char in_single_quote = 0;
    
    while (str[i])
    {
        if (str[i] == -1)  // Single quote marker
            in_single_quote = !in_single_quote;
        else if (str[i] == '$' && !in_single_quote)
            return 1;  // Found $ outside single quotes
        i++;
    }
    return 0;
}

// ============================================================================
// ENVIRONMENT VARIABLE HELPERS
// ============================================================================

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
    return "";  // Return empty string if variable not found
}

int is_valid_var_name_char(char c)
{
    return (ft_isalpha(c) || ft_isdigit(&c) || c == '_');
}

// ============================================================================
// STEP 3: VARIABLE EXPANSION
// ============================================================================

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
    char *var_name = gc_calloc(var_len + 1);
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

// ============================================================================
// STEP 4: REMOVE QUOTE MARKERS
// ============================================================================

void remove_quote_markers(t_string *input, t_string *output)
{
    if (!input || !output)
        return;
        
    int i = 0;
    while (i < input->len)
    {
        // Copy everything except quote markers
        if (input->str[i] != -1 && input->str[i] != -2)
            string_append_char(output, input->str[i]);
        i++;
    }
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

void process_token_expansion(Token *token, t_env *env_vars)
{
    if (!token || !token->str)
        return;
    
    // STEP 1: Mark quotes with special characters
    mark_quotes(token->str);
    
    // STEP 2: Check if expansion is needed (optimization)
    if (!needs_expansion(token->str))
    {
        // No variables to expand, just remove quote markers
        t_string *input = string_init(ft_strlen(token->str) + 1);
        t_string *output = string_init(ft_strlen(token->str) + 1);
        
        string_append_str(input, token->str);
        remove_quote_markers(input, output);
        
        // Replace token string
        // free(token->str);
        token->str = ft_strdup(output->str);
        
        //string_free(input);
        //string_free(output);
        return;
    }
    
    // STEP 3: Expand variables
    t_string *expanded = string_init(ft_strlen(token->str) * 2); // Estimate size
    expand_variables(token->str, expanded, env_vars);
    
    // STEP 4: Remove quote markers from expanded result
    t_string *final_result = string_init(expanded->len + 1);
    remove_quote_markers(expanded, final_result);
    
    // STEP 5: Replace original token string with final result
    free(token->str);
    token->str = ft_strdup(final_result->str);
    
    // Cleanup
    //string_free(expanded);
    //string_free(final_result);
}

// ============================================================================
// USAGE EXAMPLE
// ============================================================================

/*
void expand_all_word_tokens(t_list *token_list, t_env *env_vars)
{
    t_list *cur = token_list;
    
    while (cur)
    {
        if (cur->token->type == T_WORD)
            process_token_expansion(cur->token, env_vars);
        cur = cur->next;
    }
}
*/

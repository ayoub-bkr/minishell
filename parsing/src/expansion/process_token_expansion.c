#include "../../../minishell.h"

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
        free(token->str);
        token->str = ft_strdup(output->str);
        
        string_free(input);
        string_free(output);
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
    string_free(expanded);
    string_free(final_result);
}

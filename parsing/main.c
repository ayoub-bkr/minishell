/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:21:17 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 02:21:20 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int pipe_syntax_error(t_list *head)
{
	if (!head)
		return (0);
	t_list *cur = head;
	int i = 0;
	TokenType prev_token_type;
	while (cur)
	{
		if (cur->token->type == T_PIPE)
		{
			if (i == 0)
			{	
				printf("[+] syntax error\n");
				printf("{-} pipe cannot be first node\n");
				return (0);
			}
			if (prev_token_type == T_PIPE)
			{	
				printf("[+] syntax error\n");
				printf("{-} multiple pipes after each other\n");
				return (0);
			}
			if (!cur->next || cur->next->token->type != T_WORD)
			{
				printf("[+] syntax error\n");
				printf("{-} pipe without next command\n");
				return (0);
			}
		}
		prev_token_type = cur->token->type;
		i++;
		cur = cur->next; 
	}
	return (1);
}

int redir_syntax_error(t_list *head)
{
	if (!head)
		return (0);
	t_list *cur = head;
	TokenType pttype = T_WORD; // previous token type 
	TokenType cttype; // current toek type
	while (cur)
	{
		cttype = cur->token->type;
		if (cttype == T_RED_OUT || cttype == T_RED_IN || cttype == T_APPEND || cttype == T_HEREDOC)
		{
			if (pttype == T_RED_OUT || pttype == T_RED_IN || pttype == T_APPEND || pttype == T_HEREDOC)
			{
				printf("[+] syntax error\n");
				printf("{-} multiple redirections after each other\n");
				return 0;
			}
			if (!cur->next || cur->next->token->type != T_WORD)
			{
				printf("[+] syntax error\n");
				printf("{-} redirection without target\n");
				return 0;
			}
		}
		pttype = cur->token->type;
		cur = cur->next;
	}
	return (1);
}

int syntax_error(t_list *head)
{
	int status = 1;
	status = pipe_syntax_error(head);
	if (!status)
		return (0);
	status = redir_syntax_error(head);
	if (!status)
		return (0);
	status = quote_syntax_error(head);
	if (!status)
		return (0);
	return (status);
}
	
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
void free_token_list(t_list **head)
{
    t_list *current = *head;
    t_list *next;
    
    while (current)
    {
        next = current->next;
        
        // Free the token string (only if it exists - metacharacters have NULL str)
        if (current->token->str)
            free(current->token->str);
        
        // Free the token itself
        free(current->token);
        
        // Free the list node
        free(current);
        
        current = next;
    }
    
    *head = NULL;
}


int  init(t_list **head, t_env *env_vars)
{
	char	*input;
	int status = 1;

	input = readline("minishell$ ");
	if (!input)
	{
		rl_clear_history();
		write(1, "exit\n", 5);
		// gc_calloc(-1);
		exiting(g_exit_status);
	}
	else
		add_history(input);
	tokenize(input, head);
	status = syntax_error(*head);
	if (!status)
	{
		free(input);
		return (0);
	}
	expand_all_word_tokens(*head, env_vars);
	free(input);
	return (status);
}

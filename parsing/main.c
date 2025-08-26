#include "../minishell.h"

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

int pipe_syntax_error(t_list *head)
{
	// TODO: handle if pipe is last node
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
	TokenType pttype;
	TokenType cttype;
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

int init(t_list **head, t_env *env_vars)
{
	// char *input = "echo'sjid'|echo -n";
	//input = "   ls -l | cat file.txt >> here.txt |||||    \"okey \" here\" nice\" right\" word\" something\"\"\"\"   <<<<<<<";
	// input = "   echo |    \"okey \" here\" nice\" right\" word\" something\"\"\"\"   > echo << end";
	// TODO: handle these cases
	//input = "   \"\"\"\" ";
	//input = "   l\"s\"" ";
	//input = "   ls -l | cat file.txt >> here.txt     \"echo \"\"hello world\"\"\"\"\"\"\"   ";
	char	*input;
	int status = 1;

	input = readline("minishell$ ");
	if (!input)
	{
		rl_clear_history();
		write(1, "exit\n", 5);
		exit(0);
	}
	else
		add_history(input);
	// TODO: you have to know each return of each function!
	// if it doesn't work as expected move to next command after 
	// you set everything to default
	tokenize(input, head);
	status = syntax_error(*head);
	if (!status)
	{
		free_token_list(head);
		free(input);
		return (0);
	}
	expand_all_word_tokens(*head, env_vars);
	free(input);
	return (status);
}

// void	cmd_freeing(t_command **command)
// {
// 	t_command	*tmp;
// 	int			i;

// 	while (*command)
// 	{
// 		tmp = (*command)->next;
// 		i = 0;
// 		while ((*command)->args[i])
// 			free((*command)->args[i++]);
// 		free((*command)->args);
// 		free(*command);
// 		*command = tmp;
// 	}
// 	*command = NULL;
// }

void	print_kolchi(t_command *command)
{
	int	i = 0;

	while (command)
	{
		i = 0;
		printf("-------\n");
		while (command->args[i])
			printf("args : %s\n", command->args[i++]);
		while (command->redir)
		{
			printf("file : %s\n type : %d\n", command->redir->file, command->redir->type);
			command->redir = command->redir->next;
		}
		command = command->next;
	}
	
}

// int main()
// {
// 	t_list *head;
// 	t_command *command;
// 	command = NULL;
// 	head = NULL;
// 	// printf("cmd: %s\n", input);

// 	while (1)
// 	{
// 		init(&head);
// 		parsing(&command, head);
// 		print_kolchi(command);
// 		cmd_freeing(&command);
// 		free(head);
// 		head = NULL;
// 	}
	

// 	// print_list(head);
// 	// debug_print_all(head, command);
// 	return (0);
// }

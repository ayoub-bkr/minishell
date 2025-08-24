#include "../minishell.h"

// Print redirection type as string
const char *redir_type_str(int type)
{
    switch (type)
    {
        case 0: return "INPUT (<)";
        case 1: return "OUTPUT (>)";
        case 2: return "APPEND (>>)";
        case 3: return "HEREDOC (<<)";
        default: return "UNKNOWN";
    }
}

// Print token type as string
const char *token_type_str(TokenType type)
{
    switch (type)
    {
        case T_WORD: return "WORD";
        case T_PIPE: return "PIPE";
        case T_RED_IN: return "RED_IN";
        case T_RED_OUT: return "RED_OUT";
        case T_APPEND: return "APPEND";
        case T_HEREDOC: return "HEREDOC";
        default: return "UNKNOWN";
    }
}

// Print the token list (what you had before but enhanced)
void print_tokens(t_list *head)
{
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("                    TOKEN LIST                         \n");
    printf("═══════════════════════════════════════════════════════\n");
    
    if (!head)
    {
        printf("❌ No tokens found!\n");
        return;
    }
    
    int count = 0;
    t_list *cur = head;
    while (cur)
    {
        printf("[%d] %-10s → \"%s\"\n", 
               count, 
               token_type_str(cur->token->type), 
               cur->token->str ? cur->token->str : "(NULL)");
        cur = cur->next;
        count++;
    }
    printf("📊 Total tokens: %d\n", count);
}

// Print all redirections for a command
void print_redirections(t_redir *redir)
{
    if (!redir)
    {
        printf("    📋 No redirections\n");
        return;
    }
    
    printf("    📋 Redirections:\n");
    int redir_count = 0;
    t_redir *cur = redir;
    while (cur)
    {
        printf("      [%d] %s → file: \"%s\"\n", 
               redir_count,
               redir_type_str(cur->type),
               cur->file ? cur->file : "(NULL)");
        if (cur->type == 3) // heredoc
            printf("           heredoc_fd: %d\n", cur->heredoc_fd);
        cur = cur->next;
        redir_count++;
    }
}

// Print all arguments for a command
void print_arguments(char **args)
{
    if (!args)
    {
        printf("    💬 No arguments\n");
        return;
    }
    
    printf("    💬 Arguments:\n");
    int i = 0;
    while (args[i])
    {
        printf("      [%d] \"%s\"\n", i, args[i]);
        i++;
    }
    if (i == 0)
        printf("      (empty args array)\n");
}

// Enhanced command printing function
void print_commands(t_command *head)
{
    printf("\n═══════════════════════════════════════════════════════\n");
    printf("                   COMMAND LIST                        \n");
    printf("═══════════════════════════════════════════════════════\n");
    
    if (!head)
    {
        printf("❌ No commands found!\n");
        return;
    }
    
    int cmd_count = 0;
    t_command *cur = head;
    
    while (cur)
    {
        printf("\n🔸 Command %d:\n", cmd_count);
        
        // Print arguments
        print_arguments(cur->args);
        
        // Print redirections
        print_redirections(cur->redir);
        
        // Print pipe status
        printf("    🔗 Pipe out: %s\n", cur->pipe_out ? "YES" : "NO");
        
        // Show connection to next command
        if (cur->next)
            printf("    ➡️  Connected to next command\n");
        else
            printf("    🏁 Last command in pipeline\n");
        
        cur = cur->next;
        cmd_count++;
    }
    
    printf("\n📊 Total commands: %d\n", cmd_count);
    printf("═══════════════════════════════════════════════════════\n");
}

// All-in-one debugging function
void debug_print_all(t_list *tokens, t_command *commands)
{
    printf("\n🐛 MINISHELL DEBUG OUTPUT 🐛\n");
    
    // Print tokens first
    print_tokens(tokens);
    
    // Then print parsed commands
    print_commands(commands);
    
    printf("\n✅ Debug output complete!\n\n");
}

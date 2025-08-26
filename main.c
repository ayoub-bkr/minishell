#include "minishell.h"

int	g_exit_status = 0;

void	ctrl_c(int s)
{
	(void)s;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	cmd_freeing(t_command **command)
{
	t_command	*tmp;
	int			i;

	while (*command)
	{
		tmp = (*command)->next;
		i = 0;
		// while ((*command)->args[i])
		// 	free((*command)->args[i++]);
		// free((*command)->args);
		// free(*command);
		*command = tmp;
	}
	*command = NULL;
}

void	executing(t_command **command, t_env **env_vars)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (bi_checker((*command)->args[0]))
	{
		if ((*command)->redir)
			redirecting((*command)->redir);
		bi_handler(command, env_vars);
	}
	else
		ext_handler(*command, *env_vars);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	minishell_main(t_command **command, t_env **env_vars, t_list **head)
{
	if ((*command)->next)
		piping(*command, env_vars);
	else
		executing(command, env_vars);
	if (*command)
		cmd_freeing(command);
	if (*head)
		free_token_list(head);
	*head = NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_env		*env_vars;
	t_command	*command;
	t_list		*head;
	if (ac != 1 && av[1])
		return (0);
	env_vars = NULL;
	command = NULL;
	head = NULL;
	while (*envp)
		env_lstaddback(&env_vars, *envp++);
	create_key_value_pairs(env_vars);
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		init(&head, env_vars);
		parsing(&command, head);
		heredoc_init(command);
		if (!command)
			continue ;
		minishell_main(&command, &env_vars, &head);
	}
	return (0);
}

#include "minishell.h"

void	ext_handler(t_command *command, t_env *env_vars)
{
	pid_t	pid;
	char	**new_envp;
	char	*path;

	pid	= fork();
	if (!pid)
	{
		path = ft_strjoin("/bin/", command->args[0]);
		new_envp = env_filling(env_vars);
		if (command->redir)
            redirecting(command->redir);
		execve(path, command->args, new_envp);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

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

	tmp = NULL;
	while (*command)
	{
		if ((*command)->next)
			tmp = (*command)->next;
		else
			tmp = NULL;
		i = 0;
		while ((*command)->args[i])
			free((*command)->args[i++]);
		free((*command)->args);
		free(*command);
		*command = tmp;
	}
	*command = NULL;
}

void print_key_value_pairs(t_env *env_vars)
{
	t_env *cur = env_vars;
	while (cur)
	{
		printf("value: %s\n", cur->value);
		cur = cur->next;
	}
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
	while(*envp)
		env_lstaddback(&env_vars, *envp++);

	create_key_value_pairs(env_vars);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!init(&head, env_vars))
		{
			head = NULL;
			continue ;
		}
		parsing(&command, head);
		heredoc_init(command);
		if (!command)
			continue;
		if (command->next)
			piping(command, &env_vars);
		else
		{
			int saved_stdin  = dup(STDIN_FILENO);
			int saved_stdout = dup(STDOUT_FILENO);
			if (bi_checker(command->args[0]))
			{
        		if (command->redir)
            		redirecting(command->redir);
        		bi_handler(&command, &env_vars);
			}
			else if (!access(ft_strjoin("/bin/", command->args[0]), F_OK))
				ext_handler(command, env_vars);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
		}
		if (command)
			cmd_freeing(&command);
		if (head)
			free_token_list(&head);  // ✅ Correct
		head = NULL;
	}
}

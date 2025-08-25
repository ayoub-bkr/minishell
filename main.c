#include "minishell.h"

char	*get_path(t_env *env_vars, char *cmd)
{
	t_env	*tmp;
	char	**paths;

	tmp = env_vars;
	paths = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->var, "PATH") == 2)
		{
			tmp->var += 5;
			paths = ft_split(tmp->var, ':');
		}
		tmp = tmp->next;
	}
	if (!paths)
		return (NULL);
	while (*paths)
	{
		*paths = ft_strjoin(*paths, "/");
		if (!access(ft_strjoin(*paths, cmd), F_OK))
			return (ft_strjoin(*paths, cmd));
		paths++;
	}
	return (NULL);
}

void	ext_handler(t_command *command, t_env *env_vars)
{
	pid_t	pid;
	char	**new_envp;
	char	*path;

	pid	= fork();
	if (!pid)
	{
		if (ft_strchr(command->args[0], '/'))
			path = command->args[0];
		else
			path = get_path(env_vars, command->args[0]);
		if (command->redir)
			redirecting(command->redir);
		if (path)
			new_envp = env_filling(env_vars);
		if (execve(path, command->args, new_envp) == -1)
		{
			printf("%s: command not found\n", command->args[0]);
			exit(1);
		}
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

	while (*command)
	{
		tmp = (*command)->next;
		i = 0;
		while ((*command)->args[i])
			free((*command)->args[i++]);
		free((*command)->args);
		free(*command);
		*command = tmp;
	}
	*command = NULL;
}

// void	print_kolchi(t_command *command)
// {
// 	int	i = 0;

// 	while (command)
// 	{
// 		i = 0;
// 		printf("-------\n");
// 		while (command->args[i])
// 			printf("args : %s\n", command->args[i++]);
// 		while (command->redir)
// 		{
// 			printf("file : %s\n type : %d\n", command->redir->file, command->redir->type);
// 			command->redir = command->redir->next;
// 		}
// 		command = command->next;
// 	}
	
// }

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
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!init(&head))
		{
			head = NULL;
			continue ;
		}
		// init(&head);
		parsing(&command, head);
		heredoc_init(command);

		// print_kolchi(command);
		// cmd_freeing(&command);
		// command = NULL;
		// head = NULL;
		// continue ;
		
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
			else
				ext_handler(command, env_vars);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
		}
		if (command)
			cmd_freeing(&command);
		if (head)
			free_token_list(&head);
		head = NULL;
	}
}

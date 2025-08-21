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

int	main(int ac, char **av, char **envp)
{
	t_env		*env_vars;
	t_command	*command;

	if (ac != 1 && av[1])
		return (0);
	env_vars = NULL;
	command = NULL;
	while(*envp)
		env_lstaddback(&env_vars, *envp++);
	while (1)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		init(&command, &env_vars);
		if (!command)
			continue;
		if (command->next)
			piping(command, &env_vars);
		else
		{
			if (bi_checker(command->args[0]))
			{
				int	save_in = dup(STDIN_FILENO);
        		int	save_out = dup(STDOUT_FILENO);
        		if (command->redir)
            		redirecting(command->redir);
        		bi_handler(&command, &env_vars);
        		dup2(save_in, STDIN_FILENO);
        		dup2(save_out, STDOUT_FILENO);
        		close(save_in);
        		close(save_out);
			}
			else if (!access(ft_strjoin("/bin/", command->args[0]), F_OK))
				ext_handler(command, env_vars);
		}
		cmd_freeing(&command);
	}
}

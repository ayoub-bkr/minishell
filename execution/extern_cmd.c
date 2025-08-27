#include "../minishell.h"

void	exiting(int status)
{
	g_exit_status = status;
	gc_calloc(-1);
	exit(status);
}

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

void	ext_child(t_command *command, t_env *env_vars)
{
	char	**new_envp;
	char	*path;

	path = NULL;
	new_envp = NULL;
	if (ft_strchr(command->args[0], '/'))
		path = command->args[0];
	else
		path = get_path(env_vars, command->args[0]);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", command->args[0]);
		exiting(127);
	}
	if (command->redir)
		redirecting(command->redir);
	new_envp = env_filling(env_vars);
	if (execve(path, command->args, new_envp) == -1)
	{
		fprintf(stderr, "%s: command not found\n", command->args[0]);
		exiting(127);
	}
}

int	ext_handler(t_command *command, t_env *env_vars)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (!pid)
		ext_child(command, env_vars);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (1);
}

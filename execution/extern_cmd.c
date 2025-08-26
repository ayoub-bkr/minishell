#include "../minishell.h"

void	exiting(int status)
{
	g_exit_status = status;
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

void	ext_handler(t_command *command, t_env *env_vars)
{
	pid_t	pid;
	char	**new_envp;
	char	*path;

	pid = fork();
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
			fprintf(stderr, "%s: command not found\n", command->args[0]);
			exiting(1);
		}
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

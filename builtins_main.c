#include "minishell.h"

void	ft_putstr(char *str, int nl)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	if (nl)
		write(1, "\n", 1);
}

void	ft_putstrs(char **strs, int nl)
{
	int	i;
	int	j;

	j = 0;
	while (strs[j])
	{
		i = 0;
		while (strs[j][i])
			write(1, &strs[j][i++], 1);
		j++;
	}
	if (nl)
		write(1, "\n", 1);
}

char	*cd_home(t_env *env_vars, char *str)
{
	int		i;
	t_env	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		i = 0;
		while (str[i] && str[i] != '=')
		{
			if (str[i] != tmp->var[i])
				break ;
			i++;
		}
		if (tmp->var[i] == '=')
			return (&tmp->var[i + 1]);
		tmp = tmp->next;
	}
	return (NULL);
}

void	bi_cd(char **args, t_env *env_vars)
{
	char *dir;

	if (!args[1])
	{
		dir = cd_home(env_vars, "HOME");
		if (!dir)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return ;
		}
	}
	else
		dir = args[1];
	chdir(dir);
}

void	bi_handler(t_command **command, t_env **env_vars)
{
	char	cwd_buf[4096];

	if (ft_strcmp((*command)->args[0], "export"))
		expo_handler(command, env_vars);
	else if (ft_strcmp((*command)->args[0], "env") && !(*command)->args[1])
		env_printing(*env_vars, 0);
	else if (ft_strcmp((*command)->args[0], "unset"))
		env_lstremove(env_vars, &(*command)->args[1]);
	else if (ft_strcmp((*command)->args[0], "pwd"))
		ft_putstr(getcwd(cwd_buf, sizeof(cwd_buf)), 1);
	else if (ft_strcmp((*command)->args[0], "cd"))
		bi_cd((*command)->args, *env_vars);
	else if (ft_strcmp((*command)->args[0], "echo"))
	{
		if (ft_strcmp((*command)->args[1], "-n"))
			ft_putstrs(&(*command)->args[2], 0);
		else
			ft_putstrs(&(*command)->args[1], 1);
	}
	else if (ft_strcmp((*command)->args[0], "exit"))
	{
		write(1, "exit\n", 5);
		exit(0);
	}
}

int	bi_checker(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "env") || ft_strcmp(command, "export")
		|| ft_strcmp(command, "unset") || ft_strcmp(command, "cd")
		|| ft_strcmp(command, "echo") || ft_strcmp(command, "pwd")
		|| ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

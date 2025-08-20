#include "minishell.h"

void	exp_handler(t_command **command, t_env **env_vars)
{
	char	*tmp;

	if (!exp_equal((*command)->args[1])
		|| exp_already((*command)->args[1], *env_vars))
		return ;
	tmp = ft_strdup((*command)->args[1]);
	if (tmp)
		env_lstaddback(env_vars, tmp);
}

void	bi_handler(t_command **command, t_env **env_vars)
{
	char	cwd_buf[1024];

	if (ft_strcmp((*command)->args[0], "export"))
		exp_handler(command, env_vars);
	else if (ft_strcmp((*command)->args[0], "env"))
		env_printing(*env_vars);
	else if (ft_strcmp((*command)->args[0], "unset"))
		env_lstremove(env_vars, (*command)->args[1]);
	else if (ft_strcmp((*command)->args[0], "pwd"))
		ft_putstr(getcwd(cwd_buf, sizeof(cwd_buf)), 1);
	else if (ft_strcmp((*command)->args[0], "cd"))
		chdir((*command)->args[1]);
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

#include "../minishell.h"

void	bi_pwd(void)
{
	char	*cwd_buf;

	cwd_buf = getcwd(NULL, 0);
	if (!cwd_buf)
	{
		perror("getcwd");
		g_exit_status = 1;
		return ;
	}
	ft_putstr(cwd_buf, 1);
	free(cwd_buf);
	g_exit_status = 0;
}

void	bi_exit(char **args)
{
	int	status;

	if (args[1])
	{
		if (!args[1][0])
		{
			ft_putstr_fd("exit: : numeric argument required", 1, 2);
			exiting(2);
		}
		if (!ft_isdigit(args[1]) && !((args[1][0] == '+' || args[1][0] != '-')
			&& ft_isdigit(&(args[1][1]))))
		{
			ft_putstr_fd("exit: : numeric argument required", 1, 2);
			exiting(2);
		}
		if (args[2])
		{
			ft_putstr_fd("exit: too many arguments", 1, 2);
			exiting(1);
		}
		status = atoi(args[1]);
	}
	else
		status = g_exit_status;
	exiting(status);
}

void	bi_echo(char **args)
{
	int	i;
	int	n;
	int	j;

	if (!args)
		return ;
	i = 1;
	n = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			n = 0;
			i++;
		}
		else
			break ;
	}
	ft_putstrs(&args[i], n);
}

int	bi_handler(t_command **command, t_env **env_vars)
{
	int	status;

	status = 0;
	redirecting((*command)->redir);
	if (ft_strcmp((*command)->args[0], "export"))
	{
		expo_handler(command, env_vars);
		status = g_exit_status;
	}
	else if (ft_strcmp((*command)->args[0], "cd"))
	{
		bi_cd((*command)->args, *env_vars);
		status = g_exit_status;
	}
	else if (ft_strcmp((*command)->args[0], "exit"))
		bi_exit((*command)->args);
	else if (ft_strcmp((*command)->args[0], "env") && !(*command)->args[1])
		env_printing(*env_vars, 0);
	else if (ft_strcmp((*command)->args[0], "unset"))
		env_lstremove(env_vars, &(*command)->args[1]);
	else if (ft_strcmp((*command)->args[0], "pwd"))
		bi_pwd();
	else if (ft_strcmp((*command)->args[0], "echo"))
		bi_echo((*command)->args);
	return (status);
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

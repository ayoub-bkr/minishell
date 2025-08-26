/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:30:45 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/24 19:30:47 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	bi_pwd(void)
{
	char	*cwd_buf;

	cwd_buf = getcwd(NULL, 0);
	if (!cwd_buf)
		perror("getcwd");
	else
	{
		ft_putstr(cwd_buf, 1);
		free(cwd_buf);
	}
}

void	bi_exit(char **args)
{
	int	status;

	status = 0;
	fprintf(stdout, "exit\n");
	if (args[1])
	{
		if (!ft_isdigit(args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exiting(2);
		}
		if (args[2])
		{
			fprintf(stderr, "exit: too many arguments\n");
			g_exit_status = 1;
			return ;
		}
		status = atoi(args[1]);
	}
	exiting(status);
}

void	bi_echo(char **args)
{
	int	i;
	int	n;
	int	j;

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

void	bi_handler(t_command **command, t_env **env_vars)
{
	redirecting((*command)->redir);
	if (ft_strcmp((*command)->args[0], "export"))
		expo_handler(command, env_vars);
	else if (ft_strcmp((*command)->args[0], "env") && !(*command)->args[1])
		env_printing(*env_vars, 0);
	else if (ft_strcmp((*command)->args[0], "unset"))
		env_lstremove(env_vars, &(*command)->args[1]);
	else if (ft_strcmp((*command)->args[0], "pwd"))
		bi_pwd();
	else if (ft_strcmp((*command)->args[0], "cd"))
		bi_cd((*command)->args, *env_vars);
	else if (ft_strcmp((*command)->args[0], "echo"))
		bi_echo((*command)->args);
	else if (ft_strcmp((*command)->args[0], "exit"))
		bi_exit((*command)->args);
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

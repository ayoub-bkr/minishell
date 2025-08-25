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
		ft_putstr(cwd_buf, 1);
}

void	bi_echo(char **args)
{
	int	i;

	i = 1;
	if (ft_strcmp(args[i], "-n"))
	{
		while (ft_strcmp(args[i], "-n"))
			i++;
		ft_putstrs(&args[i], 0);
	}
	else
		ft_putstrs(&args[i], 1);
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

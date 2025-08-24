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

char	*bi_cd_home(t_env *env_vars, char *str)
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
	char	*dir;

	if (!args[1])
	{
		dir = bi_cd_home(env_vars, "HOME");
		if (!dir)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return ;
		}
	}
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		if (errno == ENOENT)
			printf("cd: %s: No such file or directory\n", dir);
		else if (errno == EACCES)
			printf("cd: %s: Permission denied\n", dir);
		else if (errno == ENOTDIR)
			printf("cd: %s: Not a directory\n", dir);
		else
			printf("cd: %s: %s\n", dir, strerror(errno));
		return ;
	}
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

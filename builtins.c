#include "minishell.h"

int	equal_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	ft_lstaddback(t_env **head, char *str)
{
	t_env	*tmp;
	t_env	*m;

	m = malloc(sizeof(t_env));
	if (!m)
		return ;
	if (!*head || !(*head)->var)
		*head = m;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = m;
	}
	m->var = str;
	m->next = NULL;
}

void	ft_lstremove(t_env **head, char *str)
{
	t_env	*tmp;
	t_env	*freeing;

	tmp = *head;
	if (ft_strcmp(tmp->var, str))
	{
		freeing = tmp;
		*head = tmp->next;
		free(freeing);
		return ;
	}
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->var, str) == 2)
		{
			freeing = tmp->next;
			tmp->next = tmp->next->next;
			free(freeing);
			return ;
		}
		else
			tmp = tmp->next;
	}
}

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

void    env_printing(t_env *env_vars)
{
	t_env	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		if (equal_sign(tmp->var))
			ft_putstr(tmp->var, 1);
		tmp = tmp->next;
	}
}

int		exp_equal(char *input)
{
	if (!input)
		return (0);
	while (*input)
	{
		if (*input == '=')
			return (1);
		input++;
	}
	return (0);
}

int		exp_already(char *input, t_env *command)
{
	int		i;
	t_env	*tmp;

	if (!input)
		return (1);
	tmp = command;
	while (tmp)
	{
		i = 0;
		while (input[i] && input[i] != '=')
		{
			if (input[i] != tmp->var[i])
				break ;
			i++;
		}
		if (input[i] == '=' && tmp->var[i] == '=')
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	bi_handler(t_command **command, t_env **env_vars)
{
	char	cwd_buf[1024];

	if (ft_strcmp((*command)->args[0], "export") && exp_equal((*command)->args[1]) && !exp_already((*command)->args[1], *env_vars))
		ft_lstaddback(env_vars, (*command)->args[1]);
	else if (ft_strcmp((*command)->args[0], "env"))
		env_printing(*env_vars);
	else if (ft_strcmp((*command)->args[0], "unset"))
		ft_lstremove(env_vars, (*command)->args[1]);
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

int		bi_checker(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "env") || ft_strcmp(command, "export") || ft_strcmp(command, "unset")
		|| ft_strcmp(command, "cd") || ft_strcmp(command, "echo")
		|| ft_strcmp(command, "pwd") || ft_strcmp(command, "exit"))
		return (1);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:14:04 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/27 20:14:06 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

void	ctrl_c(int s)
{
	(void)s;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	executing(t_command **command, t_env **env_vars)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (bi_checker((*command)->args[0]))
	{
		if ((*command)->redir)
			redirecting((*command)->redir);
		status = bi_handler(command, env_vars);
	}
	else
		status = ext_handler(*command, *env_vars);
	g_exit_status = status;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	minishell_main(t_command **command, t_env **env_vars, t_list **head)
{
	if ((*command)->next)
		piping(*command, env_vars);
	else
		executing(command, env_vars);
	*command = NULL;
	*head = NULL;
}

void	anti_norm(t_command **command, t_env **env_vars, t_list **head,
		char **envp)
{
	*env_vars = NULL;
	*command = NULL;
	*head = NULL;
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	create_key_value_pairs(*env_vars);
	while (*envp)
		env_lstaddback(env_vars, *envp++);
}

int	main(int ac, char **av, char **envp)
{
	t_env		*env_vars;
	t_command	*command;
	t_list		*head;

	if (ac != 1 && av[1])
		return (0);
	anti_norm(&command, &env_vars, &head, envp);
	while (1)
	{
		head = NULL;
		command = NULL;
		if (!init(&head, env_vars))
		{
			head = NULL;
			continue ;
		}
		parsing(&command, head);
		heredoc_init(command);
		if (!command)
			continue ;
		minishell_main(&command, &env_vars, &head);
	}
	return (g_exit_status);
}

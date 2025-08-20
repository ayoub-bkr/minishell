#include "minishell.h"

void	cmd_lstaddback(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!new)
		return ;
	new->pipe_out = 0;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		tmp->pipe_out = 1;
	}
}

int	count_tokens(char **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

t_redir *redir_new(char *file, int type)
{
    t_redir *r = malloc(sizeof(t_redir));
    if (!r)
        return NULL;
    r->file = file;
    r->type = type;
    r->next = NULL;
    return r;
}

void redir_addback(t_redir **head, t_redir *new)
{
    t_redir *tmp;
    if (!*head)
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

t_command *parsing(char *cmd_str)
{
    t_command *cmd = malloc(sizeof(t_command));
    int i = 0, j = 0;
    char **tokens = ft_split(cmd_str, ' '); // split by space
    cmd->args = malloc(sizeof(char *) * (count_tokens(tokens) + 1));
    cmd->redir = NULL;
    cmd->pipe_out = 0;
    cmd->next = NULL;

    while (tokens[i])
    {
        if (strcmp(tokens[i], "<") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 0));
        else if (strcmp(tokens[i], ">") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 1));
        else if (strcmp(tokens[i], ">>") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 2));
        else if (strcmp(tokens[i], "<<") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 3));
        else
            cmd->args[j++] = ft_strdup(tokens[i]);
        i++;
    }
    cmd->args[j] = NULL;
    return cmd;
}

char	*ft_lstgetvar(t_env *command, char *str)
{
	int	i;

	while (command)
	{
		i = 0;
		while (command->var[i] == str[i] && str[i])
			i++;
		if (command->var[i] == '=')
			return (&command->var[i + 1]);
		command = command->next;
	}
	return ("");
}

char	*replace_variable(t_env *env_vars, char *input)
{
	int	i;
	int	j;
	char	*var;
	char	*new;

	new = "";
	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	j = 1;
	if (input[i] == '$')
	{
		while (input[i + j] != ' ' && input[i + j])
			j++;
		var = ft_lstgetvar(env_vars, ft_substr(input, i + 1, j));
		new = ft_strjoin(ft_substr(input, 0, i), var);
		i += j;
		j = i;
		while (input[j])
			j++;
		new = ft_strjoin(new, ft_substr(input, i, j));
	}
	return (new);
}

void	ft_lstfree(t_env **head)
{
	t_env	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

void	init(t_command **command, t_env **env_vars)
{
	char	*input;
	char	**cmds;
	int		i;
	input = readline("minishell$ ");
	if (!input)
	{
		ft_lstfree(env_vars);
		rl_clear_history();
		write(1, "exit\n", 5);
		exit(0);
	}
	else
		add_history(input);
	if (ft_strchr(input, '$'))
		input = replace_variable(*env_vars, input);
	cmds = ft_split(input, '|');
	i = 0;
	while (cmds[i])
	{
		t_command	*new;
	
		new = parsing(cmds[i]);
		new->next = NULL;
		cmd_lstaddback(command, new);
		i++;
	}
}
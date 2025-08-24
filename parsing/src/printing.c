#include "../minishell.h"

void print_list(t_list *head)
{
	printf("_______________________________________________________\n");
	if (!head)
	{
		printf("error: exiting...\n");
		exit(0);
	}
	while (head)
	{
		printf("%s is of type %d\n", head->token->str, head->token->type);
		head = head->next;
	}
}

void print_command(t_command *head)
{
	if (!head)
		printf("error\n");
	t_command *cur = head;
	int i = 0;
	while (cur)
	{
		printf("%d\n", i);
		i++;
		cur = cur->next;
	}
}

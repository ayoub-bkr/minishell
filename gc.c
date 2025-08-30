#include "minishell.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		*((char *)s + i++) = 0;
}

void	free_all(t_gc **head)
{
	t_gc	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free((*head)->ptr);
		free(*head);
		*head = tmp;
	}
	*head = NULL;
}

void	*f_calloc(int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, size);
	return (ptr);
}

void	*gc_calloc(int size)
{
	static t_gc	*head;
	static t_gc	*last;
	t_gc		*tmp;
	void		*block;

	if (size == -1)
		return (free_all(&head), NULL);
	if (!head)
	{
		head = (t_gc *)f_calloc(sizeof(t_gc));
		last = head;
	}
	else
	{
		tmp = (t_gc *)f_calloc(sizeof(t_gc));
		last->next = tmp;
		last = tmp;
	}
	block = f_calloc(size);
	last->ptr = block;
	return (block);
}

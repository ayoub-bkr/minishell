#include "../../minishell.h"

void *ft_memcpy(void *dest, void *src, int n)
{
	unsigned const char *s;
	unsigned char *d;

	d = (unsigned char *)dest;
	s = (unsigned const char *)src;

	while (n--)
		*d++ = *s++;
	return (dest);
}

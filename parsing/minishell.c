// this is the actual main file
//
//
// get the main function to call the read line function
	// this function should initialize the head for the token linked list
// get that input from readline to be processed by a
	// "process input function"
// the pc_in_func takes in the t_string made from input and the head
// 		for the linked list
#include "minishell.h"

int main(void)
{
	char *input;
	while (1)
	{
		input = readline("prompt $ ");
		if (!input)
			break;
		pc(input);
		if (*input == '\0')
		{
			free(input);
			break;
		}
		free(input);
	}
	return (0);
}

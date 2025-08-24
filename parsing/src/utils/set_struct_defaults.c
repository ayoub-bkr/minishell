#include "../../minishell.h"

void set_def(t_string *str)
{
	str->start = 0;
	str->end = 0;
	str->peek = 0;
	str->del = '\0';
	str->quote = '\0';
	str->q_pos = 0;
	str->w_q_len = 0;
}

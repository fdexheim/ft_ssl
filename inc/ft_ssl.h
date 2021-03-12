#ifndef FT_SSL_H
# define FT_SSL_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../libft/inc/libft.h"

typedef struct				s_ssl_flags
{
	bool					p;
	bool					q;
	bool					r;
	bool					s;
}							t_ssl_flags;

typedef struct				s_ssl_env
{
	int						argc;
	char					**argv;
	t_ssl_flags				flags;
}							t_ssl_env;

typedef struct				s_ssl_command
{
	char					*command_name;
	void					(*f)(t_ssl_env *);
}							t_ssl_command;

void						usage(void);
void						close_env(t_ssl_env *env);
void						parse_command(t_ssl_env *env);

#endif

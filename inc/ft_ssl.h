#ifndef FT_SSL_H
# define FT_SSL_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../libft/inc/libft.h"

#define FT_SSL_INPUT_BUFF_SIZE 4096

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
	size_t					input_size;
	size_t					allocated_size;
	size_t					hash_size;
	char					*file_path;
	void					(*command)(struct s_ssl_env*, char **);
}							t_ssl_env;

typedef struct				s_ssl_arg_flag
{
	char					*flag;
	uint32_t				nb_args;
	void					(*flag_handler)(t_ssl_env *, char *);
}							t_ssl_arg_flags;

typedef struct				s_ssl_command
{
	char					*command_name;
	void					(*command)(t_ssl_env *, char **);
}							t_ssl_command;

void						dump_buffer(void *buff, size_t size);
void						buffer_join(void *input, void *add,
	size_t input_size, size_t add_size);
void						*bootleg_realloc(void *src, size_t old_size,
size_t new_size);

void						md5_command(t_ssl_env *env, char **args);
void						parse_md5(t_ssl_env *env, char **args);

void						sha256_command(t_ssl_env *env, char **args);
void						parse_sha256(t_ssl_env *env, char **args);

char						*gather_full_input(t_ssl_env *env,
	size_t section_size);
void						usage(void);
void						close_env(t_ssl_env *env);
void						parse_command(t_ssl_env *env, char **args);

#endif

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
	char					*s_arg;
}							t_ssl_flags;


typedef struct				s_ssl_env
{
	int						argc;
	char					**argv;
	t_ssl_flags				flags;
	size_t					input_size;
	size_t					allocated_size;
	size_t					hash_size;
	char					**full_command;
	char					**file_args;
	void					(*command)(struct s_ssl_env*, char **);
}							t_ssl_env;


typedef struct				s_ssl_command
{
	char					*command_name;
	void					(*command)(t_ssl_env *, char **);
}							t_ssl_command;


typedef struct				s_ssl_arg_flag
{
	char					*flag;
	uint32_t				nb_args;
	void					(*flag_handler)(t_ssl_env *, char **);
}							t_ssl_arg_flags;


void						dump_buffer(void *buff, size_t size);
void						buffer_join(void *input, void *add,
	size_t input_size, size_t add_size);
void						*bootleg_realloc(void *src, size_t old_size,
size_t new_size);


void						display_md5(t_ssl_env *env, char *src, void *state, bool string_mode);
void						command_md5(t_ssl_env *env, char **args);
void						parse_md5(t_ssl_env *env, char **args);
void						process_block_md5(uint32_t *block, uint32_t *state);


void						command_sha256(t_ssl_env *env, char **args);
void						parse_sha256(t_ssl_env *env, char **args);
void						process_block_sha256(uint32_t *block, uint32_t *state);


char						*gather_full_input(t_ssl_env *env, char *path);
void						env_soft_reset(t_ssl_env *env);
void						usage(void);
void						close_env(t_ssl_env *env);


void						parse_command(t_ssl_env *env, char **args);
bool						check_nb_args_required(char **args, uint32_t nb_arg);


static const t_ssl_command		g_commands[] = {
	{ "md5", command_md5 },
	{ "sha256", command_sha256 },
	{ NULL, NULL }
};


#endif

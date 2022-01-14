#ifndef FT_SSL_H
# define FT_SSL_H

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../libft/inc/libft.h"

#define FT_SSL_INPUT_BUFF_SIZE 4096

typedef struct				s_ssl_flags
{
	bool					a;
	bool					d;
	bool					e;
	bool					k;
	char					*k_arg;
	bool					p;
	char					*p_arg;
	bool					P;
	bool					q;
	bool					r;
	bool					s;
	char					*s_arg;
	bool					i;
	char					*file_arg;
	bool					o;
	char					*file_arg_out;
	bool					v;
	char					*v_arg;
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


typedef struct				s_ssl_data
{
	void					*data;
	size_t					size;
	size_t					allocated_size;
}							t_ssl_data;


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


typedef enum				ssl_output_mode
{
	SSL_JUST_HASH = 0,
	SSL_STDIN_INPUT = 1,
	SSL_STRING_INPUT = 2,
	SSL_FILE_INPUT = 4
}							e_ssl_output_mode;



void						display_hash(t_ssl_env *env, char *src, char *hash_name,
	t_ssl_data *output, e_ssl_output_mode mode);
void						dump_buffer(void *buff, size_t size);
void						buffer_join(void *input, void *add,
	size_t input_size, size_t add_size);
void						*memdup(void *src, size_t size);
void						*bootleg_realloc(void *src, size_t old_size,
size_t new_size);


void						command_base64(t_ssl_env *env, char **args);
void						process_input_base64(t_ssl_data *input, t_ssl_data *output,
	bool decrypt);
void						command_des(t_ssl_env *env, char **args);
void						command_md5(t_ssl_env *env, char **args);
void						process_input_md5(t_ssl_data *input, t_ssl_data *output);
void						command_sha224(t_ssl_env *env, char **args);
void						process_input_sha224(t_ssl_data *input, t_ssl_data *output);
void						command_sha256(t_ssl_env *env, char **args);
void						process_input_sha256(t_ssl_data *input, t_ssl_data *output);
void						command_sha512(t_ssl_env *env, char **args);
void						process_input_512(t_ssl_data *input, t_ssl_data *output);


bool						gather_full_input(t_ssl_data *input, char *path);
char						*get_data_as_str(t_ssl_data *data);
void						data_soft_reset(t_ssl_data *data);
t_ssl_data					*get_new_data_struct();
t_ssl_data					*copy_ssl_data(t_ssl_data *src);
void						clean_data_struct(t_ssl_data *toclean);
void						usage(void);
void						close_env(t_ssl_env *env);


void						parse_command(t_ssl_env *env, char **args);
bool						check_nb_args_required(char **args, uint32_t nb_arg);


static const t_ssl_command		g_commands[] = {
	{ "md5", command_md5 },
	{ "sha224", command_sha224 },
	{ "sha256", command_sha256 },
	{ "sha512", command_sha512 },
	{ "base64", command_base64 },
	{ "des", command_des },
	{ "des-cbc", command_des },
	{ "des-ecb", command_des },
	{ NULL, NULL }
};


#endif

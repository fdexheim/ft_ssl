#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void				*pad_buffer_base64(t_ssl_env *env, void *src)
{
	char				*ret;
	size_t				new_size;
	size_t				block_size = env->flags.d == true ? 4 : 3;

	if (env->input_size % block_size != 0)
	{
		new_size = env->input_size + (block_size - (env->input_size % block_size));
	}
	else
	{
		new_size = env->input_size;
	}
	ret = bootleg_realloc(src, env->input_size, new_size + 1);
	ret[new_size] = '\0';
	env->input_size = new_size;

	return (ret);
}

//------------------------------------------------------------------------------
static char				*process_input_base64(void *input, size_t input_size, bool decrypt)
{
	const size_t		block_size = decrypt == true ? 4 : 3;
	const size_t		output_block_size = decrypt == true ? 3 : 4;
	const size_t		nb_blocks = input_size / block_size;
	const size_t		output_size = nb_blocks * output_block_size;
	char				*output;

	if ((output = malloc(output_size + 1)) == NULL)
	{
		ft_putstr("Malloc failure");
		exit(EXIT_FAILURE);
	}
	ft_bzero(output, output_size + 1);
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_base64(input + (i * block_size),
			output + (i * output_block_size), decrypt);
	}
	return (output);
}

//------------------------------------------------------------------------------
static void				exec_base64(t_ssl_env *env, char *input)
{
	char			*output;

	output = process_input_base64(input, env->input_size, env->flags.d);
	display_base64(env, output);
	free(input);
	free(output);
	env_soft_reset(env);
}

//------------------------------------------------------------------------------
void				command_base64(t_ssl_env *env, char **args)
{
	char			*input;

	parse_base64(env, args);
	if (env->flags.i == true)
	{
		input = gather_full_input(env, env->flags.file_arg);
	}
	else if (env->flags.s == true)
	{
		input = ft_strdup(env->flags.s_arg);
		env->input_size = ft_strlen(input);
	}
	else
		input = gather_full_input(env, NULL);
	input = pad_buffer_base64(env, input);
	exec_base64(env, input);
}

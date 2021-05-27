#include "../../inc/ft_ssl.h"

static void				special_print_input(void *input, size_t size, bool backslashn)
{
	for (size_t i = 0; i < size; i += 64)
	{
		write(1, input + i, 64);
		if (backslashn == true)
			write(1, "\n", 1);
	}
}

//------------------------------------------------------------------------------
static void				*pad_buffer_base64(t_ssl_env *env, void *src)
{
	char				*ret;
	size_t				new_size;
	size_t				block_size = env->flags.d == true ? 4 : 3;

	special_print_input(src, env->input_size, false);

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
	if (env->flags.i == true && env->flags.d == true)
	{
		char		*file_ret;
		size_t		j = 0;

		file_ret = malloc(env->input_size);
		for (size_t i = 0; i < env->input_size; i++)
		{
			if (ret[i] != '\n' || i == env->input_size)
			{
				file_ret[j] = ret[i];
				j++;
			}
		}
		env->input_size = j;
		free(ret);
		special_print_input(file_ret, env->input_size, true);
		return (file_ret);
	}

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

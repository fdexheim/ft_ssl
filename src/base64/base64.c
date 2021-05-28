#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
static void				*pad_buffer_base64(t_ssl_env *env, void *src)
{
	char				*ret;
	size_t				new_size;
	size_t				block_size = env->flags.d == true ? 4 : 3;

	new_size = env->input_size + block_size;
	ret = bootleg_realloc(src, env->input_size, new_size + 1);
	ret[new_size] = '\0';
	if (env->flags.d == true)
	{
		char		*file_ret;
		size_t		j = 0;

		if ((file_ret = malloc(env->input_size + block_size)) == NULL)
		{
			ft_putstr("[Error] Malloc failure\n");
			exit(EXIT_FAILURE);
		}
		ft_bzero(file_ret, env->input_size);
		for (size_t i = 0; i < env->input_size; i++)
		{
			if (ret[i] != '\n')
			{
				file_ret[j] = ret[i];
				j++;
			}
		}
		env->input_size = j;
		free(ret);
		return (file_ret);
	}
	return (ret);
}

//------------------------------------------------------------------------------
static char				*process_input_base64(t_ssl_env *env, void *input, size_t input_size, bool decrypt)
{
	const size_t		block_size = decrypt == true ? 4 : 3;
	const size_t		output_block_size = decrypt == true ? 3 : 4;
	const size_t		pad_size = input_size % block_size == 0 ?
		0 : block_size - (input_size % block_size);
	const size_t		nb_blocks = pad_size == 0 ? input_size / block_size : (input_size / block_size) + 1;
	size_t				output_size = nb_blocks * output_block_size;
	char				*output;

	if ((output = malloc(output_size + 1)) == NULL)
	{
		ft_putstr("[Error] Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(output, output_size + 1);
	for (size_t i = 0; i < nb_blocks; i++)
	{
		if (i == nb_blocks - 1)
		{
			if (decrypt == true)
			{
				char		*ptr = input + (i * block_size);
				if (ptr[3] == '=')
					output_size--;
				if (ptr[2] == '=')
					output_size--;
			}
			process_block_base64(input + (i * block_size), output + (i * output_block_size), decrypt, pad_size);
		}
		else
		{
			process_block_base64(input + (i * block_size), output + (i * output_block_size), decrypt, 0);
		}
	}
	display_base64(env, output, output_size);
	return (output);
}

//------------------------------------------------------------------------------
static void			exec_base64(t_ssl_env *env, char *input)
{
	char			*output;

	output = process_input_base64(env, input, env->input_size, env->flags.d);
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

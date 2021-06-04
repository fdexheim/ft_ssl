#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_sha224.h"

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
	128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
	at then end of the input in accordance with the rfc
*/

//------------------------------------------------------------------------------
static void				*pad_buffer_sha224(t_ssl_env *env, void *src)
{
	size_t			padding_bit_size = 0;
	size_t			input_size = env->input_size;
	const size_t	length_append_bit_size = 64;
	unsigned char	*ret;
	uint32_t		*ptr;

	padding_bit_size = (input_size * 8) % 512;
	if (padding_bit_size > 448)
		padding_bit_size = 512 - (padding_bit_size - 448);
	else
		padding_bit_size = 448 - padding_bit_size;
	padding_bit_size += length_append_bit_size;
	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_bit_size);
	ret[input_size] = 128; // append 1

	ptr = (uint32_t *)ret;
	for (uint32_t i = 0; i < (input_size + padding_bit_size) / 4; i++)
	{
		ptr[i] = ft_reverse_endianess32(ptr[i]);
	}

	ptr[((input_size + padding_bit_size / 8) / 4) - 1] = input_size * 8;

	env->input_size = input_size + padding_bit_size / 8;
	return (ret);
}

//------------------------------------------------------------------------------
static char				*process_input_sha224(void *input, size_t input_size,
	uint32_t *state)
{
	uint32_t		**blocks;
	const uint32_t	block_size = 4 * 16;
	const uint32_t	nb_blocks = input_size / block_size;

	if ((blocks = malloc(sizeof(uint32_t**) * (nb_blocks + 1))) == NULL)
	{
		ft_putstr("Malloc failure");
		exit(EXIT_FAILURE);
	}
	blocks[nb_blocks] = NULL;
	for (size_t i = 0; i < input_size / block_size; i++)
	{
		blocks[i] = input + (i * block_size);
	}
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_sha224(blocks[i], state);
	}
	free(blocks);
	return (input);
}

//------------------------------------------------------------------------------
static void				exec_sha224(t_ssl_env *env, char *input, char *src, bool string_mode)
{
	uint32_t		state[8] = {
		0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
		0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
	};

	process_input_sha224(input, env->input_size, state);
	for (int i = 0; i < 8; i++)
	{
		state[i] = ft_reverse_endianess32(state[i]);
	}
	display_sha224(env, src, state, string_mode);
	free(input);
	env_soft_reset(env);
}

//------------------------------------------------------------------------------
void				command_sha224(t_ssl_env *env, char **args)
{
	char			*input;

	parse_sha224(env, args);
	if (env->flags.p == true || (env->file_args == NULL && env->flags.s == false))
	{
		input = gather_full_input(env, NULL);
		if (env->flags.p == true)
		{
			write(1, input, env->input_size);
		}
		input = pad_buffer_sha224(env, input);
		exec_sha224(env, input, NULL, false);
	}

	if (env->flags.s == true)
	{
		input = ft_strdup(env->flags.s_arg);
		env->input_size = ft_strlen(input);
		input = pad_buffer_sha224(env, input);
		exec_sha224(env, input, env->flags.s_arg, true);
	}

	if (env->file_args != NULL)
	{
		for (uint32_t i = 0; env->file_args[i] != NULL; i++)
		{
			input = gather_full_input(env, env->file_args[i]);
			input = pad_buffer_sha224(env, input);
			exec_sha224(env, input, env->file_args[i], false);
		}
	}
}

#include "../../inc/ft_ssl.h"

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
	128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
	at then end of the input in accordance with the rfc
*/

//------------------------------------------------------------------------------
static void				*pad_buffer_sha512(t_ssl_env *env, void *src)
{
	size_t			padding_bit_size;
	size_t			input_size = env->input_size;
	const size_t	length_append_bit_size = 128;
	unsigned char	*ret;
	uint64_t		*ptr;

	padding_bit_size = (input_size * 8) % 1024;
	if (padding_bit_size > 896)
		padding_bit_size = 1024 - (padding_bit_size - 896);
	else
		padding_bit_size = 896 - padding_bit_size;
	padding_bit_size += length_append_bit_size;
	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_bit_size);
	ret[input_size] = 128; // append 1

	ptr = (uint64_t *)ret;
	for (uint32_t i = 0; i < (input_size + padding_bit_size) / 8; i++)
	{
		ptr[i] = ft_reverse_endianess64(ptr[i]);
	}

	ptr[((input_size + padding_bit_size / 8) / 8) - 1] = input_size * 8;
	env->input_size = input_size + padding_bit_size / 8;
	return (ret);
}

//------------------------------------------------------------------------------
static char				*process_input_sha512(void *input, size_t input_size,
	uint64_t *state)
{
	uint64_t		**blocks;
	const uint32_t	block_size = 8 * 16;
	const uint32_t	nb_blocks = input_size / block_size;

	if ((blocks = malloc(sizeof(uint64_t**) * (nb_blocks + 1))) == NULL)
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
		process_block_sha512(blocks[i], state);
	}
	free(blocks);
	return (input);
}

//------------------------------------------------------------------------------
static void				exec_sha512(t_ssl_env *env, char *input, char *src, bool string_mode)
{
	uint64_t		state[8] = {
		0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
		0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
		0x510e527fade682d1, 0x9b05688c2b3e6c1f,
		0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
	};

	process_input_sha512(input, env->input_size, state);
	for (int i = 0; i < 8; i++)
	{
		state[i] = ft_reverse_endianess64(state[i]);
	}
	display_sha512(env, src, state, string_mode);
	free(input);
	env_soft_reset(env);
}

//------------------------------------------------------------------------------
void				command_sha512(t_ssl_env *env, char **args)
{
	char			*input;

	parse_sha512(env, args);
	if (env->flags.p == true || (env->file_args == NULL && env->flags.s == false))
	{
		input = gather_full_input(env, NULL);
		if (env->flags.p == true)
		{
			write(1, input, env->input_size);
		}
		input = pad_buffer_sha512(env, input);
		exec_sha512(env, input, NULL, false);
	}

	if (env->flags.s == true)
	{
		input = ft_strdup(env->flags.file_arg);
		env->input_size = ft_strlen(input);
		input = pad_buffer_sha512(env, input);
		exec_sha512(env, input, env->flags.file_arg, true);
	}

	if (env->file_args != NULL)
	{
		for (uint32_t i = 0; env->file_args[i] != NULL; i++)
		{
			input = gather_full_input(env, env->file_args[i]);
			input = pad_buffer_sha512(env, input);
			exec_sha512(env, input, env->file_args[i], false);
		}
	}
}

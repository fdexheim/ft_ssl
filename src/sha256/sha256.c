#include "../../inc/ft_ssl.h"

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
	128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
	at then end of the input in accordance with the rfc
*/
/*
//------------------------------------------------------------------------------
void				*pad_buffer_sha256(t_ssl_env *env, void *src)
{
	size_t			padding_bit_size = 0;
	size_t			input_size = env->input_size;
	size_t			length_append_bit_size = 64;
	unsigned char	*ret;
	uint64_t		*size_ptr;

	padding_bit_size = (input_size * 8) % 512;
	if (padding_bit_size > 448)
		padding_bit_size = 512 - (padding_bit_size - 448);
	else
		padding_bit_size = 448 - padding_bit_size;
	padding_bit_size += length_append_bit_size;
	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_bit_size);
	size_ptr = (uint64_t *)ret;
	ret[input_size] = 128;
	size_ptr[((input_size + padding_bit_size / 8) / 8) - 1] = input_size * 8;
	env->input_size = input_size + padding_bit_size / 8;
	return (ret);
}

//------------------------------------------------------------------------------
char				*sha256_process_input(void *input, size_t input_size,
	uint32_t *state)
{
	uint32_t		**blocks;
	const uint32_t	nb_blocks = input_size / (4 * 16);

	if ((blocks = malloc(sizeof(uint32_t**) * (nb_blocks + 1))) == NULL)
	{
		ft_putstr("Malloc failure");
		exit(EXIT_FAILURE);
	}
	blocks[nb_blocks] = NULL;
	for (size_t i = 0; i < input_size / 64; i++)
	{
		blocks[i] = input + (i * 64);
	}
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_sha256(blocks[i], state);
	}
	free(blocks);
	return (input);
}
*/

//------------------------------------------------------------------------------
void				command_sha256(t_ssl_env *env, char **args)
{
	(void)env;
	(void)args;
	ft_putstr(">>sha256_command called\n");
}

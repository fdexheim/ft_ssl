#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_sha256.h"

/*
note : the values are divided/multiplied by 8 to get the correct size
in either BITS or BYTES
128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
at then end of the input in accordance with the rfc
*/

//------------------------------------------------------------------------------
static void			pad_buffer_sha256(t_ssl_data *data)
{
	size_t			padding_bit_size = 0;
	size_t			input_size = data->size;
	const size_t	length_append_bit_size = 64;
	uint8_t			*append_ptr;
	uint32_t		*size_ptr;

	padding_bit_size = (input_size * 8) % 512;
	if (padding_bit_size > 448)
		padding_bit_size = 512 - (padding_bit_size - 448);
	else
		padding_bit_size = 448 - padding_bit_size;
	padding_bit_size += length_append_bit_size;
	data->data = bootleg_realloc(data->data, input_size, input_size
			+ padding_bit_size);

	size_ptr = (uint32_t *)data->data;
	append_ptr = (uint8_t *)data->data;
	append_ptr[input_size] = 128;
	for (uint32_t i = 0; i < (input_size + padding_bit_size) / 4; i++)
	{
		size_ptr[i] = ft_reverse_endianess32(size_ptr[i]);
	}
	size_ptr[((input_size + padding_bit_size / 8) / 4) - 1] = input_size * 8;
	data->size = input_size + padding_bit_size / 8;
}

//------------------------------------------------------------------------------
void				process_input_sha256(t_ssl_data *input, t_ssl_data *output)
{
	const uint32_t	state_size = 32;
	const uint32_t	block_size = 4 * 16;
	const uint32_t	state[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	uint32_t		nb_blocks;

	pad_buffer_sha256(input);
	nb_blocks = input->size / block_size;
	if ((output->data = malloc(state_size)) == NULL)
	{
		ft_putstr("[Error] Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	output->allocated_size = state_size;
	output->size = state_size;
	ft_memcpy(output->data, state, state_size);

	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_sha256(input->data + (i * block_size), output->data);
	}

	uint32_t		*ptr = output->data;
	for (int i = 0; i < 8; i++)
	{
		ptr[i] = ft_reverse_endianess32(ptr[i]);
	}
}

//------------------------------------------------------------------------------
void				command_sha256(t_ssl_env *env, char **args)
{
	t_ssl_data		*input;
	t_ssl_data		*output;

	parse_sha256(env, args);
	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));

	if (env->flags.p == true || (env->file_args == NULL && env->flags.s == false))
	{
		gather_full_input(input, NULL);
		if (env->flags.p == true)
		{
			write(1, input, env->input_size);
		}
		process_input_sha256(input, output);
		display_hash(env, NULL, "SHA256", output, false);
		data_soft_reset(input);
		data_soft_reset(output);
	}

	if (env->flags.s == true)
	{
		input->data = ft_strdup(env->flags.s_arg);
		input->size = ft_strlen(env->flags.s_arg);
		input->allocated_size = input->size + 1;
		process_input_sha256(input, output);
		display_hash(env, env->flags.s_arg, "SHA256", output, true);
		data_soft_reset(input);
		data_soft_reset(output);
	}

	if (env->file_args != NULL)
	{
		for (uint32_t i = 0; env->file_args[i] != NULL; i++)
		{
			gather_full_input(input, env->file_args[i]);
			process_input_sha256(input, output);
			display_hash(env, env->file_args[i], "SHA256", output, false);
			data_soft_reset(input);
			data_soft_reset(output);
		}
	}
	free(input);
	free(output);
}

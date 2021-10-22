#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
static void			display_des(t_ssl_data *output, char *target)
{
	int				fd;

	if (target != NULL)
	{
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
		{
			ft_putstr("[Error] display_des() : Cannot access file output '");
			ft_putstr(target);
			ft_putstr("'\n");
			return ;
		}
		write(fd, output->data, output->size);
	}
	else
	{
		write(1, output->data, output->size);
	}
}

//------------------------------------------------------------------------------
static void			pad_buffer_des(t_ssl_data *data)
{
	size_t			excess = data->size % 8;
	size_t			pad_size = 8 - excess;
	size_t			new_data_size = data->size + pad_size;

	data->data = bootleg_realloc(data->data, data->size, new_data_size);
	data->allocated_size = new_data_size;
	data->size = new_data_size;
	for (size_t i = data->size - pad_size; i < data->size; i++)
		((uint8_t*)data->data)[i] = pad_size;
}

//------------------------------------------------------------------------------
void				process_input_des(t_ssl_data *input, t_ssl_data *output,
	uint8_t *key, uint8_t *iv, e_des_operating_mode mode)
{
	const uint32_t	block_size = 8;
	uint32_t		nb_blocks;
	t_des_subkeys	*subkeys;

	pad_buffer_des(input);
	nb_blocks = input->size / block_size;
	subkeys = get_subkeys(key);
	if ((output->data = malloc(input->size)) == NULL || subkeys == NULL)
	{
		ft_putstr("[Error] Bad malloc() in process_input_des()\n");
		exit(EXIT_FAILURE);
	}
	output->size = input->size;
	output->allocated_size = output->size;
	for (size_t i = 0; i < nb_blocks; i++)
	{
		if (ft_testbit(mode, CBC_BIT) == true) // CBC bit test for XOR with previous block
		{
			uint8_t *block = input->data + i * block_size;
			uint8_t *prev_block = (i == 0) ? iv : output->data + i * block_size - block_size;
			for (uint8_t j = 0; j < block_size; j++)
				block[j] ^= prev_block[j];
		}
		if (ft_testbit(mode, DECRYPT_BIT) == true)
		{
			process_block_des_decrypt(input->data + i * block_size,
				output->data + i * block_size, subkeys);
			break; // DEBUG REMOVE LATER
		}
		else
			process_block_des_encrypt(input->data + i * block_size,
				output->data + i * block_size, subkeys);
	}
	free_subkeys(subkeys);
}

//------------------------------------------------------------------------------
void				command_des(t_ssl_env *env, char **args)
{
	t_ssl_data		*input;
	t_ssl_data		*output;
	size_t			keys_str_size = 16; // 16  OR 48 IF DES3
	size_t			iv_str_size = 16;
	size_t			salt_str_size = 16;
	uint8_t			*keys = NULL;
	uint8_t			*iv = NULL;
	uint8_t			*salt = NULL;
	e_des_operating_mode	mode;

	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("[Error] Bad malloc() in command_des()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));
	mode = parse_des(env, args);
	if (env->flags.d == true)
		mode |= DECRYPT;

	// setup
	if ((keys = get_translated_hex_input(env->flags.k_arg, keys_str_size, "Key")) == NULL)
	{
		ft_putstr("[Error] Bad key(s)\n");
		return ;
	}
	if (ft_testbit(mode, ECB_BIT) == false
		&& (iv = get_translated_hex_input(env->flags.v_arg, iv_str_size, "IV")) == NULL)
	{
		if (keys != NULL)
			free(keys);
		ft_putstr("[Error] Bad IV\n");
		return ;
	}
	if (env->flags.s == true
		&& (iv = get_translated_hex_input(env->flags.s_arg, salt_str_size, "Salt")) == NULL)
	{
		if (keys != NULL)
			free(keys);
		if (iv != NULL)
			free(iv);
		ft_putstr("[Error] Bad Salt\n");
		return ;
	}

	// input
	if (env->flags.i == true)
		gather_full_input(input, env->flags.file_arg);
	else
		gather_full_input(input, NULL);

	// process
	process_input_des(input, output, keys, iv, mode);
	
	// output
	if (env->flags.o == true)
		display_des(output, env->flags.file_arg_out);
	else
		display_des(output, NULL);

	// cleanup
	free(keys);
	if (iv != NULL)
		free(iv);
	if (salt != NULL)
		free(salt);
	data_soft_reset(input);
	data_soft_reset(output);
	free(input);
	free(output);
}

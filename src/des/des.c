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
			ft_putstr("Cannot access file output '");
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
	size_t			new_data_size;
	size_t			excess;

	if ((excess = data->size % 8) != 0 || data->size == 0)
	{
		new_data_size = data->size + 8 - excess;
		data->data = bootleg_realloc(data->data, data->size, new_data_size);
		data->allocated_size = new_data_size;
		data->size = new_data_size;
	}
}

//------------------------------------------------------------------------------
void				process_input_des(t_ssl_data *input, t_ssl_data *output, uint8_t *key, bool decyrypt)
{
printf(">>process_input_des called\n");
	const uint32_t	block_size = 8;
//	uint8_t			tmp_block[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	uint32_t		nb_blocks;
	(void)decyrypt;
	t_des_subkeys	*subkeys;

	pad_buffer_des(input);
	nb_blocks = input->size / block_size;
	printf("input size %ld / %d = %d\n", input->size, block_size, nb_blocks);
	subkeys = get_subkeys(key);
	if ((output->data = malloc(input->size)) == NULL || subkeys == NULL)
	{
		ft_putstr("[Error] Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	output->size = input->size;
	output->allocated_size = output->size;
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_des(input->data + i * block_size,
			output->data + i * block_size, subkeys);
	}
	free_subkeys(subkeys);
	printf(">>process_input_des ended\n");
}

//------------------------------------------------------------------------------
static uint8_t		base_16(char c)
{
	const char		base[16] = "0123456789abcdef";

	for (uint8_t i = 0; i < 16; i++)
	{
		if (base[i] == c)
		{
			return (i);
		}

	}
	return (0);
}

//------------------------------------------------------------------------------
bool				lowercase_and_check_hex_sanity(char *input, size_t size)
{
	for (uint32_t i = 0; i < size; i++)
		if (input[i] >= 'A' && input[i] <= 'Z')
			input[i] += 32;
	for (uint32_t i = 0; i < size; i++)
	{
		if ((input[i] >= '0' && input[i] <= '9') || (input[i] >= 'a' && input[i] <= 'f'))
			continue ;
		ft_putstr("Invalid hex key value\n");
		return (false);
	}
	return (true);
}

//------------------------------------------------------------------------------
void				translate_key_from_hex_str(char *input, uint8_t *key, size_t expected_size)
{
	uint8_t span;
	for (uint32_t i = 0; i < expected_size; i++)
	{
		span =  base_16(input[i]);
		if (i % 2 == 0)
		{
			key[i / 2] += span << 4;
		}
		else
		{
			key[i / 2] += span;
		}
	}
}

//------------------------------------------------------------------------------
static uint8_t					*get_des_keys_input(char *hex_key)
{
	bool				three_key = false;
	uint8_t				*keys;
	const size_t		expected_size = three_key == true ? 48 : 16;
	const size_t		key_output_size = expected_size / 2;
	t_ssl_data			key_input;
	size_t				input_size;

	ft_bzero(&key_input, sizeof(t_ssl_data));
	if (hex_key == NULL)
	{
		ft_putstr("Missing key(s), please input (single string) : \n");
		gather_full_input(&key_input, NULL);
		key_input.data = realloc(key_input.data, key_input.size + 1);
		((char*)key_input.data)[key_input.size] = '\0';
		hex_key = key_input.data;
	}
	input_size = ft_strlen(hex_key);
	if (input_size > expected_size)
		ft_putstr("Hex string is too long, ignoring excess\n");
	else if (input_size < expected_size)
	{
		ft_putstr("Hex string is too short, padding with zero bytes to length\n");
		char *tmp = hex_key;
		hex_key = malloc(expected_size + 1);
		ft_bzero(hex_key, expected_size + 1);
		ft_memcpy(hex_key, tmp, input_size);
	}

	if (lowercase_and_check_hex_sanity(hex_key, input_size) == false
		|| (keys = malloc(sizeof(uint8_t) * key_output_size)) == NULL)
		return (NULL);
	ft_bzero(keys, sizeof(uint8_t) * key_output_size);
	translate_key_from_hex_str(hex_key, keys, expected_size);
	if (key_input.data != NULL)
		free(key_input.data);

/*
	// TMP KEYS REMOVE LATER
		static const uint8_t	tmp_keys[24] = {
		0x13, 0x34, 0x57, 0x79,
		0x9b, 0xbc, 0xdf, 0xf1,
		0x01, 0x02, 0x03, 0x04,
		0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c,
		0x0d, 0x0e, 0x0f, 0x00
	}; // fixed one for now will implement key initialization later

	ft_putstr("KEYs : ");
	for (uint8_t j = 0; j < key_output_size; j++)
	{
		if (keys[j] <= 0xf)
			write(1, "0", 1);
		ft_put_size_t_hex(keys[j]);
	}
	ft_putstr("\n");

//	for (uint8_t j = 0; j < key_output_size; j++)
//		keys[j] = tmp_keys[j];

	ft_putstr("KEYs : ");
	for (uint8_t j = 0; j < key_output_size; j++)
	{
		if (keys[j] <= 0xf)
			write(1, "0", 1);
		ft_put_size_t_hex(keys[j]);
	}
	ft_putstr("\n");
*/
	return keys;


}

//------------------------------------------------------------------------------
static void						get_initialisation_vector(char *src, uint8_t *iv)
{
	(void)iv;
	if (src == NULL)
	{
		
	}
	else
	{
		if (ft_strlen(src) < 16)
		{
			ft_putstr("Provided IV is too short, will be zero-padded");
		}
	}
}

//------------------------------------------------------------------------------
void				command_des(t_ssl_env *env, char **args)
{
	ft_putstr(">>command_des called\n");

	t_ssl_data		*input;
	t_ssl_data		*output;
	uint8_t			*keys;
	uint8_t			iv[8];

	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));
	ft_bzero(iv, sizeof(uint8_t) * 8);

	parse_des(env, args);
	get_initialisation_vector(env->flags.v_arg, iv);
	if ((keys = get_des_keys_input(env->flags.k_arg)) == NULL)
	{
		ft_putstr("NULL KEYS\n");
		return ;
	}
	if (env->flags.s == true)
	{
		input->data = ft_strdup(env->flags.s_arg);
		input->size = ft_strlen(env->flags.s_arg);
		input->allocated_size = ft_strlen(env->flags.s_arg) + 1;
	}
	else if (env->flags.i == true)
	{
		gather_full_input(input, env->flags.file_arg);
	}

	process_input_des(input, output, keys, env->flags.d);
	if (env->flags.o == true)
		display_des(output, env->flags.file_arg_out);
	else
		display_des(output, NULL);
	ft_putstr("\n");
	data_soft_reset(input);
	data_soft_reset(output);
	free(input);
	free(output);
	ft_putstr(">>command_des ended\n");
}

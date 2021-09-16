#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
static void			pad_buffer_des(t_ssl_data *data)
{
	size_t			new_data_size;
	size_t			excess;

	if ((excess = data->size % 8) != 0)
	{
		new_data_size = data->size + 8 - excess;
		data->data = bootleg_realloc(data->data, data->size, new_data_size);
		data->allocated_size = new_data_size;
		data->size = new_data_size;
	}
}

//------------------------------------------------------------------------------
void				process_input_des(t_ssl_data *input, t_ssl_data *output)
{
	const uint32_t	key_size = 8;
	const uint32_t	block_size = 8;
	uint32_t		nb_blocks;

	pad_buffer_des(input);
	nb_blocks = input->size / block_size;
	if ((output->data = malloc(input->size)) == NULL)
	{
		ft_putstr("[Error] Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	output->allocated_size = output->size;
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block_des(input->data + (i * block_size), output->data);
	}
}

//------------------------------------------------------------------------------
static uint8_t				**get_des_keys()
{
	static int8_t					tmp_keys[3][8] = {
		{	0x13, 0x34, 0x57, 0x79,
			0x9b, 0xbc, 0xdf, 0xf1 },
		{},
		{}
	}; // fixed one for now will implement key initialization later

	
	return (tmp);
}

//------------------------------------------------------------------------------
void				command_des(t_ssl_env *env, char **args)
{
	t_ssl_data		*input;
	t_ssl_data		*output;
	uint8_t			**keys;

	parse_des(env, args);
	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));

	free(input);
	free(output);
}

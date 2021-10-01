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
void				process_input_des(t_ssl_data *input, t_ssl_data *output, uint8_t *key, bool decyrypt)
{
printf(">>process_input_des called\n");
	const uint32_t	block_size = 8;
	uint8_t			tmp_block[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
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
	output->allocated_size = output->size;
	for (size_t i = 0; i < nb_blocks; i++)
	{
//		process_block_des(input->data + i * block_size, subkeys);
		process_block_des(tmp_block, subkeys);
		break; // debug remove late for iteration;
	}
	free_subkeys(subkeys);
	printf(">>process_input_des ended\n");
}

//------------------------------------------------------------------------------
static uint8_t					**get_des_keys_input(char *file)
{
	uint8_t						**top;
	(void)file;

	top = malloc(sizeof(uint8_t*) * 3);
	for (uint8_t i = 0; i < 3; i++)
	{
		top[i] = malloc(sizeof(uint8_t) * 8);
	}
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
			top[i][j] = g_tmp_keys[i][j];
	}
	return top;
}

//------------------------------------------------------------------------------
void				command_des(t_ssl_env *env, char **args)
{
	ft_putstr(">>command_des called\n");

	t_ssl_data		*input;
	t_ssl_data		*output;
	uint8_t			**keys;

	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));

	parse_des(env, args);
	keys = get_des_keys_input(NULL);
	if (env->flags.s == true)
	{
		input->data = ft_strdup(env->flags.s_arg);
		input->size = ft_strlen(env->flags.s_arg);
		input->allocated_size = ft_strlen(env->flags.s_arg) + 1;
	}
	process_input_des(input, output, keys[0], env->flags.d);
	free(input);
	free(output);
	ft_putstr(">>command_des ended\n");
}

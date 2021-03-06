#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"
#include "../../inc/ft_ssl_base64.h"

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
		close(fd);
	}
	else
		write(1, output->data, output->size);
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
	t_des_run_data *run_data, t_des_subkeys *subkeys, e_des_operating_mode mode)
{
	const uint32_t	block_size = 8;
	size_t			salt_offset_src = 0;
	size_t			salt_offset_dst = 0;
	t_ssl_data		*input_copy = copy_ssl_data(input);

	// handle padding and salt
	if (ft_testbit(mode, DECRYPT_BIT) == false)
	{
		pad_buffer_des(input_copy);
		if (ft_testbit(mode, ADD_SALT_BIT) == true)
			salt_offset_dst = 16;
	}
	else if (ft_testbit(mode, ADD_SALT_BIT) == true)
		salt_offset_src = 16;

	// allocate output
	output->size = input_copy->size + salt_offset_dst - salt_offset_src;
	output->allocated_size = output->size;
	if ((output->data = malloc(output->size)) == NULL)
	{
		ft_putstr("[Error] Bad malloc() in process_input_des()\n");
		exit(EXIT_FAILURE);
	}
	if (ft_testbit(mode, DECRYPT_BIT) == false && ft_testbit(mode, ADD_SALT_BIT) == true)
	{
		ft_memcpy(output->data, "Salted__", 8);
		ft_memcpy(output->data + 8, run_data->salt , 8);
	}

	uint32_t nb_blocks = (input_copy->size - salt_offset_src) / block_size;
	// iteration on all blocks
	for (size_t i = 0; i < nb_blocks; i++)
	{
		// PRE PROCESS
		if (ft_testbit(mode, CBC_BIT) == true && ft_testbit(mode, DECRYPT_BIT) == false)
		{
			uint8_t *block = input_copy->data + salt_offset_src + i * block_size;
			uint8_t *prev_block = (i == 0) ? run_data->iv : output->data + salt_offset_dst + (i - 1) * block_size;
			for (uint8_t j = 0; j < block_size; j++)
				block[j] ^= prev_block[j];
		}

		// PROCESS
		process_block_des(input_copy->data + salt_offset_src + i * block_size,
			output->data + salt_offset_dst + i * block_size,
			subkeys->k, ft_testbit(mode, DECRYPT_BIT));

		// POST PROCESS
		if (ft_testbit(mode, CBC_BIT) == true && ft_testbit(mode, DECRYPT_BIT) == true)
		{
			uint8_t *block = output->data + salt_offset_dst + i * block_size;
			uint8_t *prev_block = (i == 0) ? run_data->iv : input_copy->data + salt_offset_src + (i - 1) * block_size;
			for (uint8_t j = 0; j < block_size; j++)
				block[j] ^= prev_block[j];
		}
	}
	//	PADDING REMOVAL
	if (ft_testbit(mode, DECRYPT_BIT) == true)
		output->size -= ((uint8_t *)output->data)[output->size - 1];
	clean_data_struct(input_copy);
}

//------------------------------------------------------------------------------
static void						cleanup_command_des(char *msg,
	t_ssl_data *input, t_ssl_data *output, t_ssl_data *base64put)
{
	if (msg != NULL)
		ft_putstr(msg);
	clean_data_struct(input);
	clean_data_struct(output);
	clean_data_struct(base64put);
}

//------------------------------------------------------------------------------
void						command_des(t_ssl_env *env, char **args)
{
	t_ssl_data				*input = get_new_data_struct();
	t_ssl_data				*output = get_new_data_struct();
	t_ssl_data				*base64put = get_new_data_struct();
	t_ssl_data				*input_ptr = input;
	t_des_run_data			*run_data;
	t_des_subkeys			*subkeys;
	e_des_operating_mode	mode;
	char					*password = NULL;

	mode = parse_des(env, args);
	if (env->flags.d == true)
		mode |= DECRYPT;
	if (env->flags.k == false || (env->flags.k == true && env->flags.p == true))
		mode |= ADD_SALT;

	// create empty output file if flag set because one is created by openssl
	// even if subsequent checks fail because reasons
	if (env->flags.o == true)
	{
		int fd = open(env->flags.file_arg_out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd != -1)
			close(fd);
	}
	if ((password = get_password(env)) == NULL)
	{
		cleanup_command_des("[Error] bad get_password() routine\n", input, output, base64put);
		return ;
	}
	// INPUT
	if (gather_full_input(input, env->flags.file_arg) == false)
	{
		cleanup_command_des("[Error] Bad input gathering\n", input, output, base64put);
		free(password);
		return ;
	}
	// case when input to decrypt was base64'd during encryption
	if (env->flags.a == true && env->flags.d == true)
	{
		process_input_base64(input, base64put, env->flags.d);
		input_ptr = base64put;
	}
	// run_data will contain various essentials for encryption : salt, key, iv
	if ((run_data = get_run_data(env, mode, input_ptr, password)) == NULL)
	{
		cleanup_command_des("[Error] Bad DES run_data()\n", input, output, base64put);
		free(password);
		return ;
	}
	// PROCESS
	subkeys = allocate_subkeys();
	calculate_subkeys(subkeys, run_data->keys);
	process_input_des(input_ptr, output, run_data, subkeys, mode);
	// OUTPUT
	// case where encrypted output has to be base64'd before output
	if (env->flags.a == true && env->flags.d == false)
	{
		process_input_base64(output, base64put, env->flags.d);
		display_base64(base64put, env->flags.file_arg_out, env->flags.d);
	}
	else
		display_des(output, env->flags.file_arg_out);
	free_run_data(run_data);
	free_subkeys(subkeys);
	cleanup_command_des(NULL, input, output, base64put);
	free(password);
}

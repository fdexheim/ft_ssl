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
	t_des_run_data *run_data, t_des_subkeys *subkeys, e_des_operating_mode mode)
{
	const uint32_t	block_size = 8;
	uint32_t		nb_blocks;

	if (ft_testbit(mode, DECRYPT_BIT) == false)
		pad_buffer_des(input);
	nb_blocks = input->size / block_size;
	if ((output->data = malloc(input->size)) == NULL)
	{
		ft_putstr("[Error] Bad malloc() in process_input_des()\n");
		exit(EXIT_FAILURE);
	}
	output->size = input->size;
	output->allocated_size = output->size;
	for (size_t i = 0; i < nb_blocks; i++)
	{
		// PRE PROCESS
		if (ft_testbit(mode, CBC_BIT) == true && ft_testbit(mode, DECRYPT_BIT) == false)
		{
			uint8_t *block = input->data + i * block_size;
			uint8_t *prev_block = (i == 0) ? run_data->iv : output->data + (i - 1) * block_size;
			for (uint8_t j = 0; j < block_size; j++)
				block[j] ^= prev_block[j];
		}

		// PROCESS
		process_block_des(input->data + i * block_size,
			output->data + i * block_size, subkeys->k, ft_testbit(mode, DECRYPT_BIT));

		// POST PROCESS
		if (ft_testbit(mode, CBC_BIT) == true && ft_testbit(mode, DECRYPT_BIT) == true)
		{
			uint8_t *block = output->data + i * block_size;
			uint8_t *prev_block = (i == 0) ? run_data->iv : input->data + (i - 1) * block_size;
			for (uint8_t j = 0; j < block_size; j++)
				block[j] ^= prev_block[j];
		}
	}
	//	PADDING REMOVAL
	if (ft_testbit(mode, DECRYPT_BIT) == true)
		output->size -= ((uint8_t *)output->data)[output->size - 1];
}

//------------------------------------------------------------------------------
static void			free_run_data(t_des_run_data *data)
{
	if (data->keys != NULL)
		free(data->keys);
	if (data->iv != NULL)
		free(data->iv);
	if (data->salt != NULL)
		free(data->salt);
	if (data != NULL)
		free(data);
}

//------------------------------------------------------------------------------
void					generate_pseudorandom_salt(t_des_run_data *data)
{
//	uint8_t				buff[8]= { 0xf1, 0x0f, 0xf1, 0x0f, 0xf1, 0x0f, 0xf1, 0x0f };
	uint8_t				buff[8]= { 0x61, 0x61,  0x61,  0x61,  0x61,  0x61,  0x61,  0x61 };
	data->salt = malloc(sizeof(uint8_t) * 8);
	ft_memcpy(data->salt, buff, 8);
}

//------------------------------------------------------------------------------
t_des_run_data			*get_run_data(t_ssl_env *env, e_des_operating_mode mode)
{
	size_t				keys_str_size = 16; // 16 OR 48 ? IF DES3 ?
	size_t				iv_str_size = 16;
	size_t				salt_str_size = 16;
	t_des_run_data		*ret = NULL;

	if ((ret = malloc(sizeof(t_des_run_data))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_run_data));

	// Geting arguments when required
	// get salt
	if (env->flags.s == true
		&& (ret->iv = get_translated_hex_input(env->flags.s_arg, salt_str_size, "Salt")) == NULL)
	{
		free_run_data(ret);
		ft_putstr("[Error] Bad Salt\n");
		return NULL;
	}
	else
		generate_pseudorandom_salt(ret);
	// get key (either from flag or pbkdf)
	if (env->flags.k_arg == NULL
		&& ((ret->keys = bootleg_pbkdf(env->flags.p_arg, (char *)ret->salt, 16, 8)) == NULL))
	{
		ft_putstr("[Error] Bad pbkdf \n");
		free_run_data(ret);
		return NULL;
	}
	else if ((ret->keys == NULL)
		&& (ret->keys = get_translated_hex_input(env->flags.k_arg, keys_str_size, "Key")) == NULL)
	{
		ft_putstr("[Error] Bad key(s)\n");
		free_run_data(ret);
		return NULL;
	}
	// get IV if required
	if (ft_testbit(mode, ECB_BIT) == false
		&& (ret->iv = get_translated_hex_input(env->flags.v_arg, iv_str_size, "IV")) == NULL)
	{
		ft_putstr("[Error] Bad IV\n");
		free_run_data(ret);
		return NULL;
	}

	ft_putstr("salt=");
	print_hex_key((uint8_t *)ret->salt, 8);
	ft_putstr("\nkey =");
	print_hex_key(ret->keys, 8);
	ft_putstr("\niv  =");
	if (ret->iv)
		print_hex_key((uint8_t *)ret->iv, 8);
	ft_putstr("\n");

	return (ret);
}

//------------------------------------------------------------------------------
void						command_des(t_ssl_env *env, char **args)
{
	t_ssl_data				*input = get_new_data_struct();
	t_ssl_data				*output = get_new_data_struct();
	t_ssl_data				*base64_put = get_new_data_struct();
	t_des_run_data			*run_data;
	t_des_subkeys			*subkeys;
	e_des_operating_mode	mode;

	mode = parse_des(env, args);
	if (env->flags.d == true)
		mode |= DECRYPT;

	if ((run_data = get_run_data(env, mode)) == NULL)
	{
		ft_putstr("Bad run_data()\n");
		clean_data_struct(input);
		clean_data_struct(output);
		clean_data_struct(base64_put);
		return ;
	}
	subkeys = allocate_subkeys();
	calculate_subkeys(subkeys, run_data->keys);

	// INPUT
	input = get_new_data_struct();
	gather_full_input(input, env->flags.file_arg);

	// PROCESS
	// case when input to decrypt was base64'd during encryption
	if (env->flags.a == true && env->flags.d == true)
	{
		output = get_new_data_struct();
		process_input_base64(input, base64_put, env->flags.d);
		process_input_des(base64_put, output, run_data, subkeys, mode);
	}
	else
		process_input_des(input, output, run_data, subkeys, mode);

	// OUTPUT
	// case where encrypted output has to be base64'd before output
	if (env->flags.a == true && env->flags.d == false)
	{
		process_input_base64(output, base64_put, env->flags.d);
		display_base64(base64_put, env->flags.file_arg_out, env->flags.d);
	}
	else
		display_des(output, env->flags.file_arg_out);

	free_run_data(run_data);
	free_subkeys(subkeys);
	clean_data_struct(input);
	clean_data_struct(output);
	clean_data_struct(base64_put);
}

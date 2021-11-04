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
// STEP 1 : CALCULATE SUBKEYS
//------------------------------------------------------------------------------
static void			free_subkeys(t_des_subkeys *sk)
{
	if (sk == NULL)
		return ;
	if (sk->cd != NULL)
		free_array((void*)sk->cd);
	if (sk->cd != NULL)
		free_array((void*)sk->k);
	free(sk);
}

//------------------------------------------------------------------------------
static void				calculate_subkeys(t_des_subkeys *sk, uint8_t *key)
{
	permute(key, sk->kplus, 8, 7, g_pc1_table, 56);
	for (uint8_t j = 0; j < 8; j++)
	{
		sk->cd[0][j] = sk->kplus[j];
	}
	for (uint8_t i = 1; i < 17; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			sk->cd[i][j] = sk->cd[i - 1][j];
		}
		for (uint8_t j = 0; j < g_shift_table[i - 1]; j++)
		{
			custom_bit_lshift(sk->cd[i], 4, 7);
			custom_bit_lshift(&sk->cd[i][4], 4, 7);
		}
	}
	for (int i = 0; i < 17; i++)
		permute(sk->cd[i], sk->k[i], 7, 6, g_pc2_table, 48);
}

//------------------------------------------------------------------------------
static t_des_subkeys	*allocate_subkeys()
{
	t_des_subkeys *ret;

	if ((ret = malloc(sizeof(t_des_subkeys))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_subkeys));
	ret->cd = (uint8_t**)bootleg_calloc(17, sizeof(uint8_t) * 8);
	ret->k = (uint8_t**)bootleg_calloc(17, sizeof(uint8_t) * 8);
	ft_bzero(ret->kplus, sizeof(uint8_t) * 8);
	return (ret);
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
	if ((ret->keys = get_translated_hex_input(env->flags.k_arg, keys_str_size, "Key")) == NULL)
	{
		ft_putstr("[Error] Bad key(s)\n");
		free_run_data(ret);
		return NULL ;
	}
	if (ft_testbit(mode, ECB_BIT) == false
		&& (ret->iv = get_translated_hex_input(env->flags.v_arg, iv_str_size, "IV")) == NULL)
	{
		ft_putstr("[Error] Bad IV\n");
		free_run_data(ret);
		return NULL;
	}
	if (env->flags.s == true
		&& (ret->iv = get_translated_hex_input(env->flags.s_arg, salt_str_size, "Salt")) == NULL)
	{
		free_run_data(ret);
		ft_putstr("[Error] Bad Salt\n");
		return NULL;
	}
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

	run_data = get_run_data(env, mode);
	subkeys = allocate_subkeys();
	calculate_subkeys(subkeys, run_data->keys);

	// input
	gather_full_input(input, env->flags.file_arg);

	// process
	// case when input to decrypt was base64'd during encryption
	if (env->flags.a == true && env->flags.d == true)
	{
		process_input_base64(input, base64_put, env->flags.d);
		process_input_des(base64_put, output, run_data, subkeys, mode);
	}
	else
		process_input_des(input, output, run_data, subkeys, mode);

	// output
	// case where encrypted output has to be base64'd before output
	if (env->flags.a == true && env->flags.d == false)
	{
		process_input_base64(output, base64_put, env->flags.d);
		display_base64(base64_put, env->flags.file_arg_out, env->flags.d);
	}
	else
		display_des(output, env->flags.file_arg_out);

	// cleanup
	free_run_data(run_data);
	free_subkeys(subkeys);
	clean_data_struct(input);
	clean_data_struct(output);
	clean_data_struct(base64_put);
}

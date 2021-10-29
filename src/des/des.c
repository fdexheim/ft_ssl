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
	for (int i = 0; i < 17; i++)
	{
		if (sk->cd[i] != NULL)
			free(sk->cd[i]);
		if (sk->k[i] != NULL)
			free(sk->k[i]);
	}
	if (sk->cd != NULL)
		free(sk->cd);
	if (sk->k != NULL)
		free(sk->k);
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

	ret->cd = malloc(sizeof(uint8_t *) * 17);
	ret->k = malloc(sizeof(uint8_t *) * 17);
	if (!ret->cd || !ret->k)
	{
		ft_putstr("[Error] Bad malloc in des subkey allocation\n");
		return (NULL);
	}
	for (int i = 0; i < 17; i++)
	{
		ret->cd[i] = malloc(sizeof(uint8_t) * 8);
		ret->k[i] = malloc(sizeof(uint8_t) * 8);
		if (!ret->cd[i] || !ret->k[i])
		{
			ft_putstr("[Error] Bad malloc in des subkey allocation\n");
			return (NULL);
		}
		ft_bzero(ret->cd[i], sizeof(uint8_t) * 8);
		ft_bzero(ret->k[i], sizeof(uint8_t) * 8);
	}
	ft_bzero(ret->kplus, sizeof(uint8_t) * 8);
	return (ret);
}

//------------------------------------------------------------------------------
t_des_run_data			*get_run_data(t_ssl_env *env, e_des_operating_mode mode)
{
	size_t			keys_str_size = 16; // 16 OR 48 ? IF DES3 ?
	size_t			iv_str_size = 16;
	size_t			salt_str_size = 16;
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
	t_ssl_data				*input;
	t_ssl_data				*output;
	t_des_run_data			*run_data;
	t_des_subkeys			*subkeys;
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

	run_data = get_run_data(env, mode);
	subkeys = allocate_subkeys();
	calculate_subkeys(subkeys, run_data->keys);

	// input
	if (env->flags.i == true)
		gather_full_input(input, env->flags.file_arg);
	else
		gather_full_input(input, NULL);

	// process
	process_input_des(input, output, run_data, subkeys, mode);

	// output
	if (env->flags.o == true)
		display_des(output, env->flags.file_arg_out);
	else
		display_des(output, NULL);

	// cleanup
	free_run_data(run_data);
	free_subkeys(subkeys);
	data_soft_reset(input);
	data_soft_reset(output);
	free(input);
	free(output);
}

#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"
//#include <bsd/readpassphrase.h>
#include <readpassphrase.h>

//------------------------------------------------------------------------------
void			free_run_data(t_des_run_data *data)
{
	if (data->keys != NULL)
		free(data->keys);
	if (data->iv != NULL)
		free(data->iv);
	if (data->salt != NULL)
		free(data->salt);
	if (data->password != NULL)
		free(data->password);
	if (data != NULL)
		free(data);
}

//------------------------------------------------------------------------------
uint8_t					*generate_pseudorandom_salt()
{
	// i swear it's random you're just very unlucky
	uint8_t				buff[8]= { 0x61, 0x61,  0x61,  0x61,  0x61,  0x61,  0x61,  0x61 };
	uint8_t				*ret;
	if ((ret = malloc(sizeof(uint8_t) * 8)) == NULL)
		return NULL;
	ft_memcpy(ret, buff, 8);
	return (ret);
}

//------------------------------------------------------------------------------
static void				*run_data_error(t_des_run_data *data, char *msg)
{
	ft_putstr(msg);
	free_run_data(data);
	return (NULL);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_password(t_ssl_env *env, t_des_run_data *ret)
{
	size_t				buff_size = 256;
	char				buff_verif[buff_size];
	if (env->flags.p == true)
	{
		ret->password = ft_strdup(env->flags.p_arg);
	}
	else if (env->flags.k == false)
	{
		if ((ret->password = malloc(buff_size)) == NULL)
			return (run_data_error(ret, "[Error] password verification failed\n"));
		ft_bzero(ret->password, buff_size);
		readpassphrase("password? ", ret->password, buff_size, 0);
		readpassphrase("verif password? ", buff_verif, buff_size, 0);
		if (ft_strcmp(ret->password, buff_verif))
			return (run_data_error(ret, "[Error] password verification failed\n"));
	}
	return (ret);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_salt(t_ssl_env *env, t_des_run_data *ret,
		e_des_operating_mode mode, t_ssl_data *input)
{
	size_t				salt_str_size = 16;

	if (env->flags.s == true)
	{
		if ((ret->salt = get_translated_hex_input(env->flags.s_arg, salt_str_size, "Salt")) == NULL)
			return (run_data_error(ret, "[Error] Bad Salt\n"));
	}
	else if (ft_testbit(mode, DECRYPT_BIT) == true
		&& (input->size >= 16 && (ft_memcmp(input->data, "Salted__", 8))))
	{
		if ((ret->salt = malloc(sizeof(uint8_t) * 8)) == NULL)
			return (run_data_error(ret, "[Error] Bad Salt Magic number\n"));
		ft_memcpy(ret->salt, input->data + 8, 8);
	}
	else
	{
		if ((ret->salt = generate_pseudorandom_salt()) == NULL)
			return (run_data_error(ret, "[Error] Bad Salt pseudorandom generation\n"));
	}
	return (ret);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_key(t_ssl_env *env, t_des_run_data *ret)
{
	size_t				keys_str_size = 16; // 16 OR 48 ? IF DES3 ?

	if (env->flags.k == true)
	{
		if ((ret->keys = get_translated_hex_input(env->flags.k_arg, keys_str_size, "Key")) == NULL)
			return (run_data_error(ret, "[Error] Bad key(s)\n"));
	}
	else
	{
		if ((ret->keys = bootleg_pbkdf(env->flags.p_arg, (char *)ret->salt, 256, 8)) == NULL)
			return (run_data_error(ret, "[Error] Bad pbkdf \n"));
	}
	return (ret);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_iv(t_ssl_env *env, t_des_run_data *ret, e_des_operating_mode mode)
{
	size_t				iv_str_size = 16;

	if (ft_testbit(mode, ECB_BIT) == false)
	{
		if (env->flags.v == true)
		{
			if ((ret->iv = get_translated_hex_input(env->flags.v_arg, iv_str_size, "IV")) == NULL)
				return (run_data_error(ret, "[Error] Bad IV\n"));
		}
		else if (ret->password != NULL)
		{
			if ((ret->iv = generate_pseudorandom_salt()) == NULL)
				return (run_data_error(ret, "[Error] Bad IV\n"));
		}
		else
			return (run_data_error(ret, "iv undefined\n"));
	}
	return (ret);
}

//------------------------------------------------------------------------------
t_des_run_data			*get_run_data(t_ssl_env *env, e_des_operating_mode mode, t_ssl_data *input)
{
	t_des_run_data		*ret = NULL;

	if ((ret = malloc(sizeof(t_des_run_data))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_run_data));

	if (get_password(env, ret) == NULL)
		return NULL;
	if (get_salt(env, ret, mode, input) == NULL)
		return NULL;
	if (get_key(env, ret) == NULL)
		return NULL;
	if (get_iv(env, ret, mode) == NULL)
		return NULL;
	return (ret);
}

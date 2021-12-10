#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"
#include <bsd/readpassphrase.h>
//#include <readpassphrase.h>

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
	char				buff[buff_size];
	char				buff_verif[buff_size];

	ft_bzero(buff, buff_size);
	ft_bzero(buff_verif, buff_size);
	if (env->flags.p == true)
	{
		ret->password = ft_strdup(env->flags.p_arg);
	}
	else if (env->flags.k == false)
	{
		readpassphrase("password? ", buff, buff_size, 0);
		readpassphrase("verif password? ", buff_verif, buff_size, 0);
		if (ft_strcmp(buff, buff_verif))
			return (run_data_error(ret, "[Error] password verification failed\n"));
		if ((ret->password = ft_strdup(buff)) == NULL)
			return (run_data_error(ret, "[Error] password strdup failed\n"));
	}
/*		ft_putstr("get_password bufflen = ");
		ft_put_size_t(ft_strlen(buff));
		ft_putstr("\nget_password buff     : ");
		ft_putstr(buff);
		ft_putstr("\nget_password verif    : ");
		ft_putstr(buff_verif);
		ft_putstr("\nget_password ret->password : ");
		ft_putstr(ret->password);
		ft_putstr("\n");
*/
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
static t_des_run_data	*get_key(t_ssl_env *env, t_des_run_data *ret, uint8_t *derived)
{
	size_t				keys_str_size = 16; // 16 OR 48 ? IF DES3 ?
	size_t				keys_len = 8;

	if (env->flags.k == true)
	{
		if ((ret->keys = get_translated_hex_input(env->flags.k_arg, keys_str_size, "Key")) == NULL)
			return (run_data_error(ret, "[Error] Bad key(s)\n"));
	}
	else
	{
		if ((ret->keys = malloc(sizeof(uint8_t) * keys_len)) == NULL)
			return (run_data_error(ret, "[Error] Bad get_key malloc \n"));
		ft_memcpy(ret->keys , derived, keys_len);
	}
	return (ret);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_iv(t_ssl_env *env, t_des_run_data *ret, e_des_operating_mode mode, uint8_t *derived)
{
	size_t				iv_str_size = 16;
	size_t				iv_len = 8;

	if (ft_testbit(mode, ECB_BIT) == false)
	{
		if (env->flags.v == true)
		{
			if ((ret->iv = get_translated_hex_input(env->flags.v_arg, iv_str_size, "IV")) == NULL)
				return (run_data_error(ret, "[Error] Bad IV\n"));
		}
		else if (ret->password != NULL)
		{
			if ((ret->iv = malloc(sizeof(uint8_t) * 8)) == NULL)
				return (run_data_error(ret, "[Error] Bad IV malloc()\n"));
			ft_memcpy(ret->iv, derived, iv_len);
		}
		else
			return (run_data_error(ret, "iv undefined\n"));
	}
	return (ret);
}

//------------------------------------------------------------------------------
static void					append_passsalt(t_ssl_data *d, char *password,
	size_t password_len, uint8_t *salt, size_t salt_len)
{
	size_t old_size = d->size;
	d->size += password_len + salt_len;
	d->data = bootleg_realloc(d->data, old_size, d->size);
/*
	ft_putstr("old_size : ");
	ft_put_size_t(old_size);
	ft_putstr(" new_size : ");
	ft_put_size_t(d->size);
	ft_putstr("\n");
*/

	for (size_t j = 0; j < password_len; j++)
		((char*)d->data)[old_size + j] = password[j];
	for (size_t j = 0; j < salt_len; j++)
		((char*)d->data)[old_size + j + password_len] = salt[j];
}

//------------------------------------------------------------------------------
t_des_run_data			*get_run_data(t_ssl_env *env, e_des_operating_mode mode,
		t_ssl_data *input)
{
	t_des_run_data		*ret = NULL;
	t_ssl_data			d0;
	t_ssl_data			d1;
//	t_ssl_data			d2;
//	t_ssl_data			d3;
	ft_bzero(&d0, sizeof(t_ssl_data));
	ft_bzero(&d1, sizeof(t_ssl_data));
//	ft_bzero(&d2, sizeof(t_ssl_data));
//	ft_bzero(&d3, sizeof(t_ssl_data));
	size_t key_len = 8 * sizeof(uint8_t);


	if ((ret = malloc(sizeof(t_des_run_data))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_run_data));

	if (get_salt(env, ret, mode, input) == NULL)
		return NULL;
	if (get_password(env, ret) == NULL)
		return NULL;

	size_t pw_len = ft_strlen(ret->password);
	size_t salt_len = sizeof(uint8_t) * 8;
	append_passsalt(&d0, ret->password, pw_len, ret->salt, salt_len);
	process_input_sha256(&d0, &d1);


	if (get_key(env, ret, d1.data) == NULL)
		return NULL;
	if (get_iv(env, ret, mode, d1.data + key_len) == NULL)
		return NULL;


/*
	ft_putstr("password (str) : ");
	ft_putstr(ret->password);
	ft_putstr("\npassword (key) : ");
	print_hex_key((uint8_t *)ret->password, pw_len);
	ft_putstr("\nsalt           : ");
	print_hex_key(ret->salt, salt_len);
	ft_putstr("\n");
*/

//	append_passsalt(&d1, ret->password, pw_len, ret->salt, salt_len);
//	process_input_sha256(&d1, &d2);

//	append_passsalt(&d2, ret->password, pw_len, ret->salt, salt_len);
//	process_input_sha256(&d2, &d3);

	ft_putstr("\nd0 : ");
	print_hex_key(d0.data, d0.size);
	ft_putstr("\nd1 : ");
	print_hex_key(d1.data, d1.size);
/*	ft_putstr("\nd2 : ");
	print_hex_key(d2.data, d2.size);
	ft_putstr("\nd3 : ");
	print_hex_key(d3.data, d3.size);
*/	ft_putstr("\n");

	data_soft_reset(&d0);
	data_soft_reset(&d1);
//	data_soft_reset(&d2);
//	data_soft_reset(&d3);


	ft_putstr("salt=");
	print_hex_key((uint8_t *)ret->salt, 8);
	ft_putstr("\nkey =");
	print_hex_key(ret->keys, 8);
//	if (ft_testbit(mode, DECRYPT_BIT) == false)
	{
		ft_putstr("\niv  =");
		if (ret->iv)
			print_hex_key((uint8_t *)ret->iv, 8);
		ft_putstr("\n");
	}
	return (ret);
}

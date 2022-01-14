#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

#ifdef __APPLE__
	#include <sys/random.h>
#endif

//------------------------------------------------------------------------------
void			free_run_data(t_des_run_data *data)
{
	if (data != NULL)
	{
		if (data->keys != NULL)
			free(data->keys);
		if (data->iv != NULL)
			free(data->iv);
		if (data->salt != NULL)
			free(data->salt);
		free(data);
	}
}

//------------------------------------------------------------------------------
uint8_t					*generate_pseudorandom_salt()
{
	uint8_t				*ret;
	if ((ret = malloc(sizeof(uint8_t) * 8)) == NULL)
		return NULL;
	getentropy(ret, 8);
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
static t_des_run_data	*get_salt(t_ssl_env *env, t_des_run_data *ret,
		e_des_operating_mode mode, t_ssl_data *input)
{
	size_t				salt_str_size = 16;

	if (env->flags.s == true)
	{
		if ((ret->salt = get_translated_hex_input(env->flags.s_arg, salt_str_size, "Salt")) == NULL)
			return (run_data_error(ret, "[Error] Bad Salt\n"));
	}
	else if (ft_testbit(mode, DECRYPT_BIT) == true)
	{
		if (input->size < 24 || (ft_memcmp(input->data, "Salted__", 8)))
		{
			if (env->flags.k == false)
				return (run_data_error(ret, "[Error] missing or badly formated salt\n"));
		}
		if ((ret->salt = malloc(sizeof(uint8_t) * 8)) == NULL)
			return (run_data_error(ret, "[Error] Bad get_salt() malloc()\n"));
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
		ft_memcpy(ret->keys, derived, keys_len);
	}
	return (ret);
}

//------------------------------------------------------------------------------
static t_des_run_data	*get_iv(t_ssl_env *env, t_des_run_data *ret,
		e_des_operating_mode mode, char *password, uint8_t *derived)
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
		else if (password != NULL)
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
	size_t new_size = old_size + password_len + salt_len;
	d->data = bootleg_realloc(d->data, old_size, new_size);
	d->size = new_size;
	d->allocated_size = new_size;

	for (size_t j = 0; j < password_len; j++)
		((char*)d->data)[old_size + j] = password[j];
	for (size_t j = 0; j < salt_len; j++)
		((char*)d->data)[old_size + j + password_len] = salt[j];
}

//------------------------------------------------------------------------------
t_des_run_data			*get_run_data(t_ssl_env *env, e_des_operating_mode mode,
		t_ssl_data *input, char *password)
{
	t_des_run_data		*ret = NULL;
	t_ssl_data			d0;
	t_ssl_data			d1;
	size_t key_len = 8 * sizeof(uint8_t);
	size_t pw_len = ft_strlen(password);
	size_t salt_len = sizeof(uint8_t) * 8;

	void					(*hasher)(t_ssl_data *, t_ssl_data *);
	// adjust hashing fucntion for different dump configs at school
	// for some reason macos dumps's Openssl still use md5 by default
	#ifdef __APPLE__
		hasher = process_input_md5;
	#else
		hasher = process_input_sha256;
	#endif

	if (env->flags.k == true && env->flags.p == false)
	{
		if (env->flags.v == false && ft_testbit(mode, ECB_BIT) == false)
		{
			ft_putstr("[Error] iv undefined\n");
			return NULL;
		}
	}

	if ((ret = malloc(sizeof(t_des_run_data))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_run_data));
	ft_bzero(&d0, sizeof(t_ssl_data));
	ft_bzero(&d1, sizeof(t_ssl_data));
	if (get_salt(env, ret, mode, input) == NULL)
		return (NULL);
	append_passsalt(&d0, password, pw_len, ret->salt, salt_len);
	hasher(&d0, &d1);
	if (get_key(env, ret, d1.data) == NULL)
		return NULL;
	if (get_iv(env, ret, mode, password, d1.data + key_len) == NULL)
		return NULL;
	if (env->flags.P == true)
	{
		ft_putstr("salt=");
		print_hex_key((uint8_t *)ret->salt, 8);
		ft_putstr("\nkey =");
		print_hex_key(ret->keys, 8);
		if (ft_testbit(mode, ECB_BIT) == false)
		{
			ft_putstr("\niv  =");
			print_hex_key((uint8_t *)ret->iv, 8);
			ft_putstr("\n");
		}
	}
//	print_hex_key(d1.data, d1.size);
//	ft_putstr("\n");
	data_soft_reset(&d0);
	data_soft_reset(&d1);
	return (ret);
}

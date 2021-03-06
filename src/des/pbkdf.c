#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
// Custom implementation of PBKDF
// Note : it is currently unused as it's not required to mimic openssl's
// implementation.
//------------------------------------------------------------------------------
uint8_t			*bootleg_pbkdf(char *password, char *salt,
	size_t nb_desired_iters, size_t dk_len)
{
	uint8_t				*ret_key;
	t_ssl_data			one;
	t_ssl_data			two;
	t_ssl_data			*in_ptr = &one;
	t_ssl_data			*out_ptr = &two;
	t_ssl_data			*swap;

	ft_bzero(in_ptr, sizeof(t_ssl_data));
	ft_bzero(out_ptr, sizeof(t_ssl_data));

	size_t pw_len = ft_strlen(password);
	out_ptr->size = sizeof(uint8_t) * 8 + pw_len;
	out_ptr->data = malloc(out_ptr->size);
	for (size_t j = 0; j < pw_len; j++)
		((char*)out_ptr->data)[j] = password[j];
	for (size_t j = 0; j < 8; j++)
		((char*)out_ptr->data)[j + pw_len] = salt[j];
	for (size_t i = 0; i < nb_desired_iters; i++)
	{
		// 'swap' in and out each iter because we don't need previous ones
		swap = in_ptr;
		in_ptr = out_ptr;
		out_ptr = swap;
		data_soft_reset(out_ptr);
		process_input_sha256(in_ptr, out_ptr);
	}
	if ((ret_key = malloc(dk_len)) == NULL)
	{
		ft_putstr("[Error] bad malloc() in bootleg_pbkdf()\n");
		return NULL;
	}
	for (size_t i = 0; i < dk_len; i++)
		ret_key[i] = ((uint8_t *)out_ptr->data)[i];
	data_soft_reset(in_ptr);
	data_soft_reset(out_ptr);
	return ret_key;
}

#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"
#include <bsd/readpassphrase.h>


/*
//------------------------------------------------------------------------------
uint8_t			*prf(uint8_t *input, uint8_t *output)
{
	
}

//------------------------------------------------------------------------------
uint8_t			*f(char *password, char *salt, size_t nb_desired_iters, uint32_t i)
{
	int32_t		be_int = ft_reverse_endianess32(i);
	uint8_t		**u = bootleg_calloc(nb_desired_iters + 1, hlen);
	uint8_t		*ret;

	for (size_t j = 1; j < nb_desired_iters; j++)
	{
		if (j == 1)
		{
			
		}
		else
		{
			
		}
	}
}
*/
//------------------------------------------------------------------------------
uint8_t			*bootleg_pbkdf(t_des_run_data *data, char *password,
	char *salt, size_t nb_desired_iters, size_t dk_len)
{
	const size_t		buff_size = 256;
	char				buff[buff_size];
//	uint8_t				*ret_key;
	t_ssl_data			in;
	t_ssl_data			out;
	ft_bzero(&in, sizeof(t_ssl_data));
	ft_bzero(&out, sizeof(t_ssl_data));
//	size_t		hlen = 64;

	(void)dk_len;
	(void)data;
	(void)salt;
	(void)nb_desired_iters;

	if (password == NULL)
	{
		ft_bzero(buff, buff_size);
		password = buff;
		readpassphrase("password? ", buff, buff_size, 0);
		ft_putstr("password = '");
		ft_putstr(buff);
		ft_putstr("'\n");
		ft_putstr("salt=");
		print_hex_key((uint8_t *)salt, 8);
		ft_putstr("\n");
	}
	for (size_t i  = 1; i < nb_desired_iters; i++)
	{
		
	}


	return NULL;
}



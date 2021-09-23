#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
void			custom_bit_print(uint8_t *key, uint8_t key_size, uint8_t bit_size)
{
	for (uint8_t i = 0; i < key_size; i++)
	{
		for (uint8_t j = bit_size; j < 8; j++)
			ft_putstr(" ");
		for (int8_t k = bit_size - 1; k >= 0; k--)
		{
			if (ft_testbit(key[i], k) == true)
				write(1, "1", 1);
			else
				write(1, "0", 1);
		}
		ft_putstr(" ");
	}
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
void			custom_bit_lshift(uint8_t *key, uint8_t key_size, uint8_t bit_number_wrap_point)
{
	bool		big_wrap = false;

	for (uint8_t i = 0; i < key_size; i++)
	{
		key[i] = key[i] << 1;
		if (ft_testbit(key[i], bit_number_wrap_point) == true)
		{
			if (i == 0)
				big_wrap = true;
			else
				ft_set_bit_8(&key[i - 1], 0);
			ft_unset_bit_8(&key[i], bit_number_wrap_point);
		}
	}
	if (big_wrap)
		ft_set_bit_8(&key[key_size - 1], 0);
}

//------------------------------------------------------------------------------
void			debug_key(uint8_t *key)
{
	uint64_t *test = (uint64_t*)key;
	ft_putnbr_bits(*test, 64);
	ft_putstr("\n");
	for (uint8_t i = 0; i < 8; i++)
	{
		ft_putnbr_bits(key[i], 8);
		ft_putstr(" ");
	}

	ft_putstr("\n");
	for (uint8_t i = 0; i < 8; i++)
	{
		ft_unset_bit_8(&key[i], 7);
		ft_putnbr_bits(key[i], 8);
		ft_putstr(" ");
	}
	ft_putstr("\n");
	custom_bit_print(key, 8, 7);
	for (int i = 0; i < 56; i++)
	{
		custom_bit_lshift(key, 8, 7);
		custom_bit_print(key, 8, 7);
	}
}

//------------------------------------------------------------------------------
void			permute(uint8_t *new_key, uint8_t cell_size,
	const uint8_t *permutation_table, uint8_t permutations)
{
	
}

//------------------------------------------------------------------------------
uint8_t			***calc_subkeys(uint8_t *key)
{
	(void)key;
	return (NULL);
}

//------------------------------------------------------------------------------
void			process_block_des(uint8_t *block, uint8_t *k)
{
	uint8_t		kplus[8];
	ft_putstr(">>process_block_des called\n");

	


	(void)block;
	debug_key(k);
ft_putstr(">>process_block_des ended\n");
}

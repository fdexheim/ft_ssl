#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

/*
//------------------------------------------------------------------------------
void			custom_bit_shift(uint8_t *value, uint8_t wrap_point)
{
	//value << 1;
	if (ft_testbit(value, wrap_point) == true)
	{
		ft_set_bit(value, 0);
		ft_unset_bit(value, wrap_point);
	}
}
*/
//------------------------------------------------------------------------------
void			debug_key(uint8_t *key)
{
	for (uint8_t i = 0; i < 8; i++)
	{
//		printf("Key[%d]='%x' ", i, key[i]);
		ft_putnbr_bits(key[i], 8);
		ft_putstr(" ");
	}
	ft_putstr("\n");
	uint64_t *test = (uint64_t*)key;
	ft_putnbr_bits(*test, 64);
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
uint8_t			***calc_subkeys(uint8_t *key)
{
	(void)key;
	return (NULL);
}

//------------------------------------------------------------------------------
void			process_block_des(uint8_t *block, uint8_t *key)
{
ft_putstr(">>process_block_des called\n");
	//uint8_t		***subkeys = calc_subkeys(key);
	(void)block;
	debug_key(key);
ft_putstr(">>process_block_des ended\n");
}

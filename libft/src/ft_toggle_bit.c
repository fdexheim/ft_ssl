#include "../inc/libft.h"

void			ft_toggle_bit_8(uint8_t *value, uint8_t bit_number)
{
	*value ^= 1UL << bit_number;
}

void			ft_toggle_bit_32(uint32_t *value, uint8_t bit_number)
{
	*value ^= 1UL << bit_number;
}

void			ft_toggle_bit_64(uint64_t *value, uint8_t bit_number)
{
	*value ^= 1UL << bit_number;
}

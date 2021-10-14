#include "../inc/libft.h"

uint32_t				ft_set_bit_span(uint32_t val, uint32_t new_bit_span, uint8_t bit_start, uint8_t bit_end)
{
	uint32_t		ret = val;
	uint32_t		i = 0;
	uint32_t		span = bit_end - bit_start;

	if (bit_start > bit_end)
		return (ret);
	while (i <= span)
	{
		if (ft_testbit(new_bit_span, i) == true)
			ft_set_bit_32(&ret, bit_start + i);
		else
			ft_unset_bit_32(&ret, bit_start + i);
		bit_start++;
		i++;
	}
	return (ret);
}

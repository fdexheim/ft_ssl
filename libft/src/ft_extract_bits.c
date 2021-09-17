#include "../inc/libft.h"

uint64_t			ft_extract_bits64(uint64_t val, uint8_t bit_start, uint8_t bit_end)
{
	uint64_t		weight = 1;
	uint64_t		ret = 0;

	while (bit_start <= bit_end)
	{
		if (ft_testbit(val, bit_start) == true)
			ret += weight;
		bit_start++;
		weight *= 2;
	}
	return (ret);
}

uint32_t			ft_extract_bits(uint32_t val, uint8_t bit_start, uint8_t bit_end)
{
	uint32_t		weight = 1;
	uint32_t		ret = 0;

	while (bit_start <= bit_end)
	{
		if (ft_testbit(val, bit_start) == true)
			ret += weight;
		bit_start++;
		weight *= 2;
	}
	return (ret);
}

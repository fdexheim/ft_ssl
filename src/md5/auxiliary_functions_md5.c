#include "../../inc/ft_ssl.h"

/*
	Auxiliary functions for computing md5 as described in RFC 1321
*/

static void		print_bits(uint64_t src, size_t size)
{
	for (int i = size - 1; i >= 0; i--)
	{
		if (ft_testbit(src, i) == true)
			write(1, "1", 1);
		else
			write(1, "0", 1);
	}
}

//------------------------------------------------------------------------------
uint32_t		md5_f(uint32_t x, uint32_t y, uint32_t z) // F(X,Y,Z) = XY v not(X) Z
{
	uint32_t	ret = 0;

	ret = ((x & y) | ((~x) & z));

	write(1, "x = ", 4);
	print_bits(x, 32);
	write(1, "\ny = ", 5);
	print_bits(y, 32);
	write(1, "\nz = ", 5);
	print_bits(z, 32);
	write(1, "\nret ", 5);
	print_bits(ret, 32);
	write(1, "\n", 1);

	return (ret);
}
//------------------------------------------------------------------------------
uint32_t		md5_g(uint32_t x, uint32_t y, uint32_t z) // G(X,Y,Z) = XZ v Y not(Z)
{
	uint32_t	ret = 0;

	ret = ((x & z) | (y & (~z)));

	(void)x;
	(void)y;
	(void)z;

	return (ret);
}

//------------------------------------------------------------------------------
uint32_t		md5_h(uint32_t x, uint32_t y, uint32_t z) // H(X,Y,Z) = X xor Y xor Z
{
	uint32_t	ret = 0;

	ret = x ^ y ^ z;

	write(1, "x = ", 4);
	print_bits(x, 32);
	write(1, "\ny = ", 5);
	print_bits(y, 32);
	write(1, "\nz = ", 5);
	print_bits(z, 32);
	write(1, "\nret ", 5);
	print_bits(ret, 32);
	write(1, "\n", 1);

	return (ret);
}

//------------------------------------------------------------------------------
uint32_t		md5_i(uint32_t x, uint32_t y, uint32_t z) // I(X,Y,Z) = X xor (X v not(Z))
{
	uint32_t	ret = 0;

	(void)x;
	(void)y;
	(void)z;

	return (ret);
}

#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_md5.h"

//------------------------------------------------------------------------------
void				*pad_buffer_md5(t_ssl_env *env, void *src)
{
	size_t			padding_bit_size = 0;
	size_t			input_size = env->input_size;
	size_t			length_append_bit_size = 64;
	unsigned char	*ret;
	uint64_t		*size_ptr;

	padding_bit_size = (input_size * 8) % 512;
	if (padding_bit_size > 448)
	{
		padding_bit_size = 512 - (padding_bit_size - 448);
	}
	else
		padding_bit_size = 448 - padding_bit_size;
	padding_bit_size += length_append_bit_size;

/*
	printf("input_size      = %ld (%ld bits)\n"
			"padding_size    = %ld (%ld bits)\n"
			"input + padding = %ld (%ld bits)\n",
		input_size, input_size * 8, padding_bit_size / 8, padding_bit_size, input_size + padding_bit_size / 8, input_size * 8 + padding_bit_size);
*/

	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_bit_size);
	size_ptr = (uint64_t *)ret;
	ret[input_size] = 128;
	size_ptr[((input_size + padding_bit_size / 8) / 8) - 1] = input_size;
	env->input_size = input_size + padding_bit_size / 8;

	return (ret);
}

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
	128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
	at then end of the input in accordance with the rfc
*/

static void		debug_state(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	ft_putstr("a = ");
	ft_putnbr_bits(a, 32);
	ft_putstr("\nb = ");
	ft_putnbr_bits(d, 32);
	ft_putstr("\nc = ");
	ft_putnbr_bits(c, 32);
	ft_putstr("\nd = ");
	ft_putnbr_bits(b, 32);
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
static void		process_block(uint32_t *block, uint32_t *state)
{
	ft_putstr("processing block : \n");
	dump_buffer(block, 64);

	uint32_t	a = state[0];
	uint32_t	b = state[1];
	uint32_t	c = state[2];
	uint32_t	d = state[3];

	const uint32_t	sin_table[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	}; // precalculated sin values

	const uint32_t	shift_table[16] = {
		7, 12, 17, 22,
		5, 9, 14, 20,
		4, 11, 16, 23,
		6, 10, 15, 21
	};

	debug_state(a, b, c, d);
	FF(a, b, c, d, block[0], shift_table[0], sin_table[0]);
	debug_state(a, b, c, d);
	FF(d, a, b, c, block[1], shift_table[1], sin_table[1]);
	FF(c, d, a, b, block[2], shift_table[2], sin_table[2]);
	FF(b, c, d, a, block[3], shift_table[3], sin_table[3]);
	FF(a, b, c, d, block[4], shift_table[0], sin_table[4]);
	FF(d, a, b, c, block[5], shift_table[1], sin_table[5]);
	FF(c, d, a, b, block[6], shift_table[2], sin_table[6]);
	FF(b, c, d, a, block[7], shift_table[3], sin_table[7]);
	FF(a, b, c, d, block[8], shift_table[0], sin_table[8]);
	FF(d, a, b, c, block[9], shift_table[1], sin_table[9]);
	FF(c, d, a, b, block[10], shift_table[2], sin_table[10]);
	FF(b, c, d, a, block[11], shift_table[3], sin_table[11]);
	FF(a, b, c, d, block[12], shift_table[0], sin_table[12]);
	FF(d, a, b, c, block[13], shift_table[1], sin_table[13]);
	FF(c, d, a, b, block[14], shift_table[2], sin_table[14]);
	FF(b, c, d, a, block[15], shift_table[3], sin_table[15]);

	GG(a, b, c, d, block[1], shift_table[4], sin_table[16]);
	GG(d, a, b, c, block[6], shift_table[5], sin_table[17]);
	GG(c, d, a, b, block[11], shift_table[6], sin_table[18]);
	GG(b, c, d, a, block[0], shift_table[7], sin_table[19]);
	GG(a, b, c, d, block[5], shift_table[4], sin_table[20]);
	GG(d, a, b, c, block[10], shift_table[5], sin_table[21]);
	GG(c, d, a, b, block[15], shift_table[6], sin_table[22]);
	GG(b, c, d, a, block[4], shift_table[7], sin_table[23]);
	GG(a, b, c, d, block[9], shift_table[4], sin_table[24]);
	GG(d, a, b, c, block[14], shift_table[5], sin_table[25]);
	GG(c, d, a, b, block[3], shift_table[6], sin_table[26]);
	GG(b, c, d, a, block[8], shift_table[7], sin_table[27]);
	GG(a, b, c, d, block[13], shift_table[4], sin_table[28]);
	GG(d, a, b, c, block[2], shift_table[5], sin_table[29]);
	GG(c, d, a, b, block[7], shift_table[6], sin_table[30]);
	GG(b, c, d, a, block[12], shift_table[7], sin_table[31]);

	HH(a, b, c, d, block[5], shift_table[8], sin_table[32]);
	HH(d, a, b, c, block[8], shift_table[9], sin_table[33]);
	HH(c, d, a, b, block[11], shift_table[10], sin_table[34]);
	HH(b, c, d, a, block[14], shift_table[11], sin_table[35]);
	HH(a, b, c, d, block[1], shift_table[8], sin_table[36]);
	HH(d, a, b, c, block[4], shift_table[9], sin_table[37]);
	HH(c, d, a, b, block[7], shift_table[10], sin_table[38]);
	HH(b, c, d, a, block[10], shift_table[11], sin_table[39]);
	HH(a, b, c, d, block[13], shift_table[8], sin_table[40]);
	HH(d, a, b, c, block[0], shift_table[9], sin_table[41]);
	HH(c, d, a, b, block[3], shift_table[10], sin_table[42]);
	HH(b, c, d, a, block[6], shift_table[11], sin_table[43]);
	HH(a, b, c, d, block[9], shift_table[8], sin_table[44]);
	HH(d, a, b, c, block[12], shift_table[9], sin_table[45]);
	HH(c, d, a, b, block[15], shift_table[10], sin_table[46]);
	HH(b, c, d, a, block[2], shift_table[11], sin_table[47]);

	II(a, b, c, d, block[0], shift_table[12], sin_table[48]);
	II(d, a, b, c, block[7], shift_table[13], sin_table[49]);
	II(c, d, a, b, block[14], shift_table[14], sin_table[50]);
	II(b, c, d, a, block[5], shift_table[15], sin_table[51]);
	II(a, b, c, d, block[12], shift_table[12], sin_table[52]);
	II(d, a, b, c, block[3], shift_table[13], sin_table[53]);
	II(c, d, a, b, block[10], shift_table[14], sin_table[54]);
	II(b, c, d, a, block[1], shift_table[15], sin_table[55]);
	II(a, b, c, d, block[8], shift_table[12], sin_table[56]);
	II(d, a, b, c, block[15], shift_table[13], sin_table[57]);
	II(c, d, a, b, block[6], shift_table[14], sin_table[58]);
	II(b, c, d, a, block[13], shift_table[15], sin_table[59]);
	II(a, b, c, d, block[4], shift_table[12], sin_table[60]);
	II(d, a, b, c, block[11], shift_table[13], sin_table[61]);
	II(c, d, a, b, block[2], shift_table[14], sin_table[62]);
	II(b, c, d, a, block[9], shift_table[15], sin_table[63]);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

//------------------------------------------------------------------------------
char				*md5_process_input(void *input, size_t input_size,
	uint32_t *state)
{
	printf(">>md5_process_input called\n");
	uint32_t		**blocks;
	const uint32_t	nb_blocks = input_size / (4 * 16);

	if ((blocks = malloc(sizeof(uint32_t**) * (nb_blocks + 1))) == NULL)
	{
		ft_putstr("Malloc failure");
		exit(EXIT_FAILURE);
	}
	blocks[nb_blocks] = NULL;
	for (size_t i = 0; i < input_size / 64; i++)
	{
		blocks[i] = input + (i * 64);
	}
	for (size_t i = 0; i < nb_blocks; i++)
	{
		process_block(blocks[i], state);
	}
	free(blocks);
	return (input);
}

//------------------------------------------------------------------------------
void				md5_command(t_ssl_env *env, char **args)
{
	printf(">>md5_command called\n");

	char			*input;
	uint32_t		state[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
	unsigned char	*ptr = (unsigned char*)state;

	env->hash_size = 16;
	parse_md5(env, args);
	input = gather_full_input(env, 64);

	printf("Final input size (before padding) = %ld\n", env->input_size);
	input = pad_buffer_md5(env, input);
	printf("Final input size (after  padding) = %ld\n", env->input_size);
	dump_buffer(input, env->input_size);

	md5_process_input(input, env->input_size, state);

	free(input);

	if (env->file_path != NULL)
	{
		ft_putstr("MD5(");
		ft_putstr(env->file_path);
		ft_putstr(")= ");
	}
	for (uint32_t i = 0; i < 16; i++)
	{
		if (ptr[i] <= 0xf)
			write(1, "0", 1);
		ft_put_size_t_hex(ptr[i]);
	}
	ft_putstr("\n");
}

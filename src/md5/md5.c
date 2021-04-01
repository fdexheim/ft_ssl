#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_md5.h"

//------------------------------------------------------------------------------
uint64_t			reverse_endian_64(uint64_t src)
{
	uint64_t		ret = 0;
	unsigned char	*src_ptr = (unsigned char*)&src;
	unsigned char	*ret_ptr = (unsigned char*)&ret;;

	for (int i = 0; i < 8; i++)
	{
		ret_ptr[i] = src_ptr[7-i];
	}
//	ft_putstr("src ");
//	ft_putnbr_bits(src, 64);
//	ft_putstr("\nret ");
//	ft_putnbr_bits(ret, 64);
//	ft_putstr("\n");
	return (ret);
}

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
		padding_bit_size = 512 - (padding_bit_size - 448);
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
	size_ptr[((input_size + padding_bit_size / 8) / 8) - 1] =
		reverse_endian_64(input_size * 8);
	env->input_size = input_size + padding_bit_size / 8;

	return (ret);
}

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
	128 is 10000000 in binary for uint8_t, which allows us to add the '1' bit
	at then end of the input in accordance with the rfc
*/


//------------------------------------------------------------------------------
void				debug_state(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	ft_putstr("a = ");
	ft_put_size_t_hex(a);
	ft_putstr("\nb = ");
	ft_put_size_t_hex(b);
	ft_putstr("\nc = ");
	ft_put_size_t_hex(c);
	ft_putstr("\nd = ");
	ft_put_size_t_hex(d);
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
void				debug_state_bits(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	ft_putstr("a = ");
	ft_putnbr_bits(a, 32);
	ft_putstr("\nb = ");
	ft_putnbr_bits(b, 32);
	ft_putstr("\nc = ");
	ft_putnbr_bits(c, 32);
	ft_putstr("\nd = ");
	ft_putnbr_bits(d, 32);
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
static void			process_block(uint32_t *block, uint32_t *state)
{
	ft_putstr("processing block : \n");
	dump_buffer(block, 64);

	uint32_t		a = state[0];
	uint32_t		b = state[1];
	uint32_t		c = state[2];
	uint32_t		d = state[3];
	const uint32_t	*sht = g_md5_shift_table;
	const uint32_t	*sit = g_md5_sin_table;

ft_putstr("state   =\n");
debug_state(state[0], state[1], state[2], state[3]);
	FF(a, b, c, d, block[0], sht[0], sit[0]);
	FF(d, a, b, c, block[1], sht[1], sit[1]);
	FF(c, d, a, b, block[2], sht[2], sit[2]);
	FF(b, c, d, a, block[3], sht[3], sit[3]);

	FF(a, b, c, d, block[4], sht[0], sit[4]);
	FF(d, a, b, c, block[5], sht[1], sit[5]);
	FF(c, d, a, b, block[6], sht[2], sit[6]);
	FF(b, c, d, a, block[7], sht[3], sit[7]);

	FF(a, b, c, d, block[8], sht[0], sit[8]);
	FF(d, a, b, c, block[9], sht[1], sit[9]);
	FF(c, d, a, b, block[10], sht[2], sit[10]);
	FF(b, c, d, a, block[11], sht[3], sit[11]);


ft_putstr("operation 11 FF round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	FF(a, b, c, d, block[12], sht[0], sit[12]);

ft_putstr("operation 12 FF round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	FF(d, a, b, c, block[13], sht[1], sit[13]);

ft_putstr("operation 13 FF round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	FF(c, d, a, b, block[14], sht[2], sit[14]);

ft_putstr("operation 14 FF round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	FF(b, c, d, a, block[15], sht[3], sit[15]);

ft_putstr("End of FF round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	GG(a, b, c, d, block[1], sht[4], sit[16]);
	GG(d, a, b, c, block[6], sht[5], sit[17]);
	GG(c, d, a, b, block[11], sht[6], sit[18]);
	GG(b, c, d, a, block[0], sht[7], sit[19]);
	GG(a, b, c, d, block[5], sht[4], sit[20]);
	GG(d, a, b, c, block[10], sht[5], sit[21]);
	GG(c, d, a, b, block[15], sht[6], sit[22]);
	GG(b, c, d, a, block[4], sht[7], sit[23]);
	GG(a, b, c, d, block[9], sht[4], sit[24]);
	GG(d, a, b, c, block[14], sht[5], sit[25]);
	GG(c, d, a, b, block[3], sht[6], sit[26]);
	GG(b, c, d, a, block[8], sht[7], sit[27]);
	GG(a, b, c, d, block[13], sht[4], sit[28]);
	GG(d, a, b, c, block[2], sht[5], sit[29]);
	GG(c, d, a, b, block[7], sht[6], sit[30]);
	GG(b, c, d, a, block[12], sht[7], sit[31]);

ft_putstr("End of GG round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	HH(a, b, c, d, block[5], sht[8], sit[32]);
	HH(d, a, b, c, block[8], sht[9], sit[33]);
	HH(c, d, a, b, block[11], sht[10], sit[34]);
	HH(b, c, d, a, block[14], sht[11], sit[35]);
	HH(a, b, c, d, block[1], sht[8], sit[36]);
	HH(d, a, b, c, block[4], sht[9], sit[37]);
	HH(c, d, a, b, block[7], sht[10], sit[38]);
	HH(b, c, d, a, block[10], sht[11], sit[39]);
	HH(a, b, c, d, block[13], sht[8], sit[40]);
	HH(d, a, b, c, block[0], sht[9], sit[41]);
	HH(c, d, a, b, block[3], sht[10], sit[42]);
	HH(b, c, d, a, block[6], sht[11], sit[43]);
	HH(a, b, c, d, block[9], sht[8], sit[44]);
	HH(d, a, b, c, block[12], sht[9], sit[45]);
	HH(c, d, a, b, block[15], sht[10], sit[46]);
	HH(b, c, d, a, block[2], sht[11], sit[47]);

ft_putstr("End of HH round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

	II(a, b, c, d, block[0], sht[12], sit[48]);
	II(d, a, b, c, block[7], sht[13], sit[49]);
	II(c, d, a, b, block[14], sht[14], sit[50]);
	II(b, c, d, a, block[5], sht[15], sit[51]);
	II(a, b, c, d, block[12], sht[12], sit[52]);
	II(d, a, b, c, block[3], sht[13], sit[53]);
	II(c, d, a, b, block[10], sht[14], sit[54]);
	II(b, c, d, a, block[1], sht[15], sit[55]);
	II(a, b, c, d, block[8], sht[12], sit[56]);
	II(d, a, b, c, block[15], sht[13], sit[57]);
	II(c, d, a, b, block[6], sht[14], sit[58]);
	II(b, c, d, a, block[13], sht[15], sit[59]);
	II(a, b, c, d, block[4], sht[12], sit[60]);
	II(d, a, b, c, block[11], sht[13], sit[61]);
	II(c, d, a, b, block[2], sht[14], sit[62]);
	II(b, c, d, a, block[9], sht[15], sit[63]);

ft_putstr("End of II round :\n");
debug_state(a, b, c, d);
ft_putstr("\n");

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
//	uint32_t		state[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
	uint32_t		state[4] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
	unsigned char	*ptr = (unsigned char*)state;

	env->hash_size = 16;
	parse_md5(env, args);
	input = gather_full_input(env, 64);

//	printf("Final input size (before padding) = %ld bytes (%ld bits)\n", env->input_size, env->input_size * 8);
	input = pad_buffer_md5(env, input);
//	printf("Final input size (after  padding) = %ld bytes (%ld bits)\n", env->input_size, env->input_size * 8);
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

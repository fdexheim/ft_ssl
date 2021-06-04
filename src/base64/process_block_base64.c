#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_base64.h"

//------------------------------------------------------------------------------
static char			translate(char value)
{
	uint8_t			c;

	if (value == '=')
		return (65);
	for (c = 0; g_base64_table[c] != '\0'; c++)
	{
		if (value == g_base64_table[c])
			return (c);
	}
	ft_putstr("[Warning] this input contains characters that are not part of the base64 table\n");
	return (65);
}

//------------------------------------------------------------------------------
static void			process_block_base64_encrypt(char *input_block, char *output_block, size_t pad_size)
{
	uint64_t		value1;
	uint64_t		value2;
	uint64_t		sum;

	/* output_block[0] */
	value1 = ft_extract_bits(input_block[0], 2, 7);
	sum = value1;
	output_block[0] = g_base64_table[sum];

	/* output_block[1] */
	value1 = ft_extract_bits(input_block[0], 0, 1);
	value2 = ft_extract_bits(input_block[1], 4, 7);
	sum = value2 + (SHL(value1, 4));
	output_block[1] = g_base64_table[sum];

	/* output_block[2] */
	value1 = ft_extract_bits(input_block[1], 0, 3);
	value2 = ft_extract_bits(input_block[2], 6, 7);
	sum = value2 + (SHL(value1, 2));
	output_block[2] = g_base64_table[sum];

	/* output_block[3] */
	value1 = ft_extract_bits(input_block[2], 0, 5);
	sum = value1;
	output_block[3] = g_base64_table[sum];

	/*	Padding handling */
	if (pad_size > 0)
	{
		if (pad_size > 1)
			output_block[2] = '=';
		if (pad_size > 0)
			output_block[3] = '=';
	}
}

//------------------------------------------------------------------------------
static void			process_block_base64_decrypt(char *input_block, char *output_block)
{
	uint64_t		value1;
	uint64_t		value2;
	char			buf[4];


	for (int i = 0; i < 4; i++)
	{
		buf[i] = translate(input_block[i]);
		if (input_block[i] == '=')
			buf[i] = '\0';
	}

	/* output_block[0] */
	value1 = (SHL(buf[0], 2));
	value2 = (SHR((buf[1] & 0x30), 4));
	output_block[0] = value1 + value2;

	/* output_block[1] */
	value1 = (SHL((buf[1] & 0xf), 4));
	value2 = (SHR((buf[2] & 0x3c), 2));
	output_block[1] = value1 + value2;

	/* output_block[2] */
	value1 = (SHL((buf[2] & 0x3), 6));
	value2 = buf[3];
	output_block[2] = value1 + value2;

}

//------------------------------------------------------------------------------
void				process_block_base64(char *input_block, char *output_block, bool decrypt, size_t pad_size)
{
	if (decrypt == true)
		process_block_base64_decrypt(input_block, output_block);
	else
		process_block_base64_encrypt(input_block, output_block, pad_size);
}

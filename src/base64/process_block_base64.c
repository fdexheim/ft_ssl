#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_base64.h"
/*
static void			debug_value(char *name, uint8_t value)
{
	ft_putstr(name);
	ft_putstr(" ");
	ft_putnbr_bits(value, 8);
	ft_putstr("\n");
}
*/

//------------------------------------------------------------------------------
static void			unset_bit(uint8_t *src, uint8_t bit_number)
{
	*src &= ~(1UL << bit_number);
}

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
static void			process_block_base64_encrypt(char *input_block, char *output_block)
{
	uint64_t		value1;
	uint64_t		value2;
	uint64_t		sum;

	// output_block[0]
	value1 = ft_extract_bits(input_block[0], 2, 7);
	sum = value1;
	output_block[0] = g_base64_table[sum];

	// output_block[1]
	value1 = ft_extract_bits(input_block[0], 0, 1);
	value2 = ft_extract_bits(input_block[1], 4, 7);
	sum = value2 + (SHL(value1, 4));
	output_block[1] = g_base64_table[sum];

	// output_block[2]
	value1 = ft_extract_bits(input_block[1], 0, 3);
	value2 = ft_extract_bits(input_block[2], 6, 7);
	sum = value2 + (SHL(value1, 2));
	output_block[2] = g_base64_table[sum];

	// output_block[3]
	value1 = ft_extract_bits(input_block[2], 0, 5);
	sum = value1;
	output_block[3] = g_base64_table[sum];

	//	Padding handling
	if (input_block[1] == '\0')
		output_block[2] = '=';
	if (input_block[2] == '\0')
		output_block[3] = '=';
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
	}

	if (input_block[3] == '=')
	{
		write(1, "PAD 3\n", 6);
		for (int i = 0; i < 6; i++)
			unset_bit((uint8_t*)&buf[2], i);
	}
	if (input_block[2] == '=')
	{
		write(1, "PAD 2\n", 6);
		for (int i = 6; i < 8; i++)
			unset_bit((uint8_t*)&buf[2], i);
		for (int i = 0; i < 4; i++)
			unset_bit((uint8_t*)&buf[1], i);
	}
	if (input_block[1] == '=')
	{
		write(1, "PAD 1\n", 6);
		for (int i = 4; i < 8; i++)
			unset_bit((uint8_t*)&buf[1], i);
		for (int i = 0; i < 2; i++)
			unset_bit((uint8_t*)&buf[0], i);
	}


	// output_block[0]
	value1 = (SHL(buf[0], 2));
	value2 = (SHR((buf[1] & 0x30), 4));
	output_block[0] = value1 + value2;

	// output_block[1]
	value1 = (SHL((buf[1] & 0xf), 4));
	value2 = (SHR((buf[2] & 0x3c), 2));
	output_block[1] = value1 + value2;

	// output_block[2]
	value1 = (SHL((buf[2] & 0x3), 6));
	value2 = buf[3];
	output_block[2] = value1 + value2;

/*
	debug_value("value1", value1);
	debug_value("value2", value2);
	ft_putstr("\n");
*/

	ft_putstr("input_block : '");
	write(1, input_block, 4);
	ft_putstr("'  value : ");
	for (int i = 0; i < 4; i++)
	{
		ft_putnbr_bits(input_block[i], 8);
		ft_putstr(" (");
		ft_put_size_t(input_block[i]);
		ft_putstr(") ");
	}
	ft_putstr("\n");

	ft_putstr("buf                  value : ");
	for (int i = 0; i < 4; i++)
	{
		ft_putnbr_bits(buf[i], 8);
		ft_putstr(" (");
		ft_put_size_t(buf[i]);
		ft_putstr(") ");
	}
	ft_putstr("\n");



	ft_putstr("output_block : '");
	write(1, output_block, 3);
	ft_putstr("' value : ");
	for (int i = 0; i < 3; i++)
	{
		ft_putnbr_bits(output_block[i], 8);
		ft_putstr(" (");
		ft_put_size_t(output_block[i]);
		ft_putstr(") ");
	}
	ft_putstr("\n");


	ft_putstr("\n");
//	getchar();
}

//------------------------------------------------------------------------------
void				process_block_base64(char *input_block, char *output_block, bool decrypt)
{
	if (decrypt == true)
		process_block_base64_decrypt(input_block, output_block);
	else
		process_block_base64_encrypt(input_block, output_block);
}

#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
static uint8_t		base_16(char c)
{
	const char		base[16] = "0123456789ABCDEF";

	for (uint8_t i = 0; i < 16; i++)
		if (base[i] == c)
			return (i);
	return (0);
}

//------------------------------------------------------------------------------
void			print_hex_key(uint8_t *key, size_t size)
{
	const char		hexa[] = "0123456789ABCDEF";

	for (size_t i = 0; i < size; i++)
	{
		write(1, &hexa[key[i] / 16], 1);
		write(1, &hexa[key[i] % 16], 1);
	}
}

//------------------------------------------------------------------------------
static bool				uppercase_and_check_hex_sanity(char *input, size_t size)
{
	for (uint32_t i = 0; i < size; i++)
		if (input[i] >= 'a' && input[i] <= 'z')
			input[i] -= 32;
	for (uint32_t i = 0; i < size; i++)
	{
		if ((input[i] >= '0' && input[i] <= '9') || (input[i] >= 'A' && input[i] <= 'F'))
			continue ;
		ft_putstr("Invalid hex key value\n");
		return (false);
	}
	return (true);
}

//------------------------------------------------------------------------------
void				translate_key_from_hex_str(char *input, uint8_t *key, size_t expected_size)
{
	uint8_t span;
	for (uint32_t i = 0; i < expected_size; i++)
	{
		span =  base_16(input[i]);
		if (i % 2 == 0)
			key[i / 2] += span << 4;
		else
			key[i / 2] += span;
	}
}

//------------------------------------------------------------------------------
void				translate_hex_str_from_key(uint8_t *key, char *output, size_t expected_size)
{
	const char		hexa[] = "0123456789ABCDEF";
	for (uint32_t i = 0; i < expected_size; i += 2)
	{
		output[i * 2] = hexa[key[i / 16]];
		output[i * 2 + 1] = hexa[key[i % 16]];
	}
}

//------------------------------------------------------------------------------
uint8_t					*get_translated_hex_input(char *hex_str, size_t expected_size, char *name)
{
	t_ssl_data			hex_input;
	size_t				input_size;
	const size_t		key_output_size = expected_size / 2;
	uint8_t				*ret;

	ft_bzero(&hex_input, sizeof(t_ssl_data));
	if (hex_str == NULL)
	{
		ft_putstr(name);
		ft_putstr(" : Missing input, please input (single string)\n");
		gather_full_input(&hex_input, NULL);
		hex_input.data = realloc(hex_input.data, hex_input.size + 1);
		((char*)hex_input.data)[hex_input.size] = '\0';
		hex_str = hex_input.data;
	}
	input_size = ft_strlen(hex_str);
	if (input_size > expected_size)
	{
		ft_putstr(name);
		ft_putstr(" : Hex string is too long, ignoring excess\n");
	}
	else if (input_size < expected_size)
	{
		ft_putstr(name);
		ft_putstr(" : Hex string is too short, padding with zero bytes to length\n");
	}
	if (uppercase_and_check_hex_sanity(hex_str, input_size) == false
		|| (ret = malloc(sizeof(uint8_t) * key_output_size)) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(uint8_t) * key_output_size);
	translate_key_from_hex_str(hex_str, ret, input_size);
	if (hex_input.data != NULL)
		free(hex_input.data);
	return ret;
}



#include "../../inc/ft_ssl.h"

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

	printf("input_size      = %ld (%ld bits)\npadding_size    = %ld (%ld bits)\ninput + padding = %ld (%ld bits)\n",
		input_size, input_size * 8, padding_bit_size / 8, padding_bit_size, input_size + padding_bit_size / 8, input_size * 8 + padding_bit_size);

	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_bit_size);
	size_ptr = (uint64_t *)ret;
	ret[input_size] = 128; // adds 0b10000000 at the end of the input
	size_ptr[((input_size + padding_bit_size / 8) / 8) - 1] = input_size;
	env->input_size = input_size + padding_bit_size / 8;

	dump_buffer(ret, env->input_size);
	return (ret);
}

/*
	note : the values are divided/multiplied by 8 to get the correct size
	in either BITS or BYTES
*/

//------------------------------------------------------------------------------
char			*md5_process_input(char *input)
{
	printf(">>md5_process_input called\n");
//	uint32_t	md_buffer[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
//	uint32_t	md_buffer_save[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
	uint32_t	test = 0;

	test = md5_f(0x15843902, 0x32146543, 0x34791268);

	printf("\n");

	test = md5_h(0x15843902, 0x32146543, 0x34791268);

	printf("test = %d topkeks\n", test);

	(void)input;

	return (input);
}

//------------------------------------------------------------------------------
void			md5_command(t_ssl_env *env, char **args)
{
	printf(">>md5_command called\n");

	char		*input;
	char		*hashed;

	env->hash_size = 16;
	parse_md5(env, args);
	input = gather_full_input(env, 64);

	printf("Final input size (before padding) = %ld\n", env->input_size);
	input = pad_buffer_md5(env, input);
	printf("Final input size (after  padding) = %ld\n", env->input_size);

	hashed = md5_process_input(input);

	free(input);
	free(hashed);
}

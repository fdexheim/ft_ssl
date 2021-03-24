#include "../../inc/ft_ssl.h"

//------------------------------------------------------------------------------
void				*pad_buffer_md5(t_ssl_env *env, void *src)
{
	size_t			padding_size = 0;
	const size_t	length_append_size = (64 / 8);
	size_t			input_size = env->input_size;
	unsigned char	*ret;
	uint64_t		*size_ptr;

	padding_size = input_size % (512 / 8);
	if (padding_size == 0)
	{
		padding_size = (448 / 8);
	}
	else if (padding_size < (448 / 8))
	{
		padding_size = (448 / 8) - padding_size;
	}
	padding_size += length_append_size;
	printf("input_size = %ld | padding_size = %ld | input + padding = %ld\n",
		input_size, padding_size, input_size + padding_size);
	ret = (unsigned char *)bootleg_realloc(src, input_size, input_size
		+ padding_size);

	size_ptr = (uint64_t *)ret;
	size_ptr[(input_size + padding_size) / 8] = input_size;
	ret[input_size] = 128; // need to make sure it's in accordance with endianess
	env->input_size = input_size + padding_size;

	printf("ret = %p | size_ptr = %p\n", ret, size_ptr);
	for (size_t i = 0; i < (input_size + padding_size) / 8; i++)
	{
		printf("%lx ", size_ptr[i]);
	}
	printf("\n\n");



	dump_buffer(ret, env->input_size);

	return (ret);
}

/*
	note : the values are divided by 8 to give the correct length in BYTES
*/

//------------------------------------------------------------------------------
char			*md5_process_input(char *input)
{
	printf(">>md5_process_input called\n");

	char		*ret;
	(void)input;

	if ((ret = malloc(64)) == NULL)
	{
		ft_putstr("[ERROR] malloc failure");
		exit(EXIT_FAILURE);
	}
	return (ret);
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

#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_base64.h"

//------------------------------------------------------------------------------
static void			write_base64(int fd, char *output, bool d,
	size_t output_size)
{
	size_t			iter = output_size / 64;

	for (size_t i = 0; i < iter; i++)
	{
		write(fd, output, 64);
		if (d == false)
			write(fd, "\n", 1);
		output += 64;
	}
	if (output_size % 64 > 0)
	{
		write(fd, output, output_size % 64);
		if (d == false)
			write(fd, "\n", 1);
	}
}

//------------------------------------------------------------------------------
void				display_base64(t_ssl_data *output, char *file_output,
	bool decrypt)
{
	int				fd;

	if (file_output != NULL)
	{
		fd = open(file_output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
		{
			ft_putstr("Cannot access file output '");
			ft_putstr(file_output);
			ft_putstr("'\n");
			return ;
		}
		write_base64(fd, output->data, decrypt, output->size);
	}
	else
	{
		write_base64(1, output->data, decrypt, output->size);
	}
}

//------------------------------------------------------------------------------
static void				pad_buffer_base64(t_ssl_data *data, bool decrypt)
{
	size_t				new_size;
	const size_t		block_size = decrypt == true ? 4 : 3;
	char				*append_ptr;

	new_size = data->size + block_size;
	data->data = bootleg_realloc(data->data, data->size, new_size + 1);
	append_ptr = (char *)data->data;
	append_ptr[new_size] = '\0';
	data->allocated_size = new_size + 1;
	if (decrypt == true)
	{
		char		*file_ret;
		size_t		j = 0;

		if ((file_ret = malloc(data->size + block_size)) == NULL)
		{
			ft_putstr("[Error] Malloc failure\n");
			exit(EXIT_FAILURE);
		}
		data->allocated_size = data->size + block_size;
		ft_bzero(file_ret, data->size);
		for (size_t i = 0; i < data->size; i++)
		{
			if (append_ptr[i] != '\n')
			{
				file_ret[j] = append_ptr[i];
				j++;
			}
		}
		void *tofree = data->data;
		data->data = file_ret;
		data->size = j;
		free(tofree);
	}
}

//------------------------------------------------------------------------------
void					process_input_base64(t_ssl_data *input,
	t_ssl_data *output, bool decrypt)
{
	t_ssl_data		*input_copy = copy_ssl_data(input);
	pad_buffer_base64(input_copy, decrypt);

	const size_t		block_size = decrypt == true ? 4 : 3;
	const size_t		output_block_size = decrypt == true ? 3 : 4;
	const size_t		pad_size = input_copy->size % block_size == 0 ?
		0 : block_size - (input_copy->size % block_size);
	const size_t		nb_blocks = pad_size == 0 ?
		input_copy->size / block_size : (input_copy->size / block_size) + 1;

	output->size = nb_blocks * output_block_size;
	if ((output->data = malloc(output->size + 1)) == NULL)
	{
		ft_putstr("[Error] Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	output->allocated_size = output->size + 1;
	ft_bzero(output->data, output->size + 1);

	for (size_t i = 0; i < nb_blocks; i++)
	{
		if (i == nb_blocks - 1)
		{
			if (decrypt == true)
			{
				char		*ptr = input_copy->data + (i * block_size);
				if (ptr[3] == '=')
					output->size--;
				if (ptr[2] == '=')
					output->size--;
			}
			process_block_base64(input_copy->data + (i * block_size),
				output->data + (i * output_block_size), decrypt, pad_size);
		}
		else
		{
			process_block_base64(input_copy->data + (i * block_size),
				output->data + (i * output_block_size), decrypt, 0);
		}
	}
	clean_data_struct(input_copy);
}

//------------------------------------------------------------------------------
void				command_base64(t_ssl_env *env, char **args)
{
	t_ssl_data		*input = get_new_data_struct();
	t_ssl_data		*output = get_new_data_struct();
	parse_base64(env, args);
	if (env->flags.i == true)
	{
		gather_full_input(input, env->flags.file_arg);
	}
	else if (env->flags.s == true)
	{
		input->data = ft_strdup(env->flags.s_arg);
		input->size = ft_strlen(input->data);
		input->allocated_size = input->size + 1;
	}
	else
		gather_full_input(input, NULL);
	process_input_base64(input, output, env->flags.d);
	display_base64(output, env->flags.file_arg_out, env->flags.d);
	clean_data_struct(input);
	clean_data_struct(output);
}

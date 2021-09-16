#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_base64.h"

//------------------------------------------------------------------------------
static void			write_base64(int fd, char *output, bool d, size_t output_size)
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
static void			display_base64(t_ssl_env *env, t_ssl_data *output)
{
	int				fd;

	if (env->flags.o == true)
	{
		fd = open(env->flags.file_arg_out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
		{
			ft_putstr("Cannot access file output '");
			ft_putstr(env->flags.file_arg_out);
			ft_putstr("'\n");
			return ;
		}
		write_base64(fd, output->data, env->flags.d, output->size);
	}
	else
	{
		write_base64(1, output->data, env->flags.d, output->size);
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
		free(data->data);
		data->data = file_ret;
		data->size = j;
	}
}

//------------------------------------------------------------------------------
void					process_input_base64(t_ssl_data *input, t_ssl_data *output, bool decrypt)
{
	const size_t		block_size = decrypt == true ? 4 : 3;
	const size_t		output_block_size = decrypt == true ? 3 : 4;
	const size_t		pad_size = input->size % block_size == 0 ?
		0 : block_size - (input->size % block_size);
	const size_t		nb_blocks = pad_size == 0 ?
		input->size / block_size : (input->size / block_size) + 1;

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
				char		*ptr = input->data + (i * block_size);
				if (ptr[3] == '=')
					output->size--;
				if (ptr[2] == '=')
					output->size--;
			}
			process_block_base64(input->data + (i * block_size),
				output->data + (i * output_block_size), decrypt, pad_size);
		}
		else
		{
			process_block_base64(input->data + (i * block_size),
				output->data + (i * output_block_size), decrypt, 0);
		}
	}
}

//------------------------------------------------------------------------------
void				command_base64(t_ssl_env *env, char **args)
{
	t_ssl_data		*input;
	t_ssl_data		*output;

	parse_base64(env, args);
	if ((input = malloc(sizeof(t_ssl_data))) == NULL || (output = malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("[Error] Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(input, sizeof(t_ssl_data));
	ft_bzero(output, sizeof(t_ssl_data));

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
	pad_buffer_base64(input, env->flags.d);
	process_input_base64(input, output, env->flags.d);
	display_base64(env, output);
	data_soft_reset(input);
	data_soft_reset(output);
	free(input);
	free(output);
}

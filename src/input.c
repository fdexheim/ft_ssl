#include "../inc/ft_ssl.h"

//------------------------------------------------------------------------------
void			gather_full_input_loop(t_ssl_data *input, int fd)
{
	char		buffer[FT_SSL_INPUT_BUFF_SIZE];
	ssize_t		read_size = 0;

	ft_bzero(buffer, FT_SSL_INPUT_BUFF_SIZE);
	while ((read_size = read(fd, buffer, FT_SSL_INPUT_BUFF_SIZE)) > 0)
	{
		if (input->allocated_size < input->size + read_size)
		{
			while (input->allocated_size < input->size + read_size)
				input->allocated_size += FT_SSL_INPUT_BUFF_SIZE;
			input->data = bootleg_realloc(input->data, input->size, input->allocated_size);
		}
		buffer_join(input->data, buffer, input->size, read_size);
		input->size += read_size;
		ft_bzero(buffer, FT_SSL_INPUT_BUFF_SIZE);
	}
}

//------------------------------------------------------------------------------
void			gather_full_input(t_ssl_data *input, char *path)
{
	int			fd;

	if (path == NULL)
	{
		gather_full_input_loop(input, 0);
	}
	else
	{
		if ((fd = open(path, O_RDONLY)) < 0)
		{
			ft_putstr("[ERROR] Failed to open file '");
			ft_putstr(path);
			ft_putstr("'\n");
			return ;
		}
		gather_full_input_loop(input, fd);
		close(fd);
	}
}

//------------------------------------------------------------------------------
void				data_soft_reset(t_ssl_data *data)
{
	if (data->data != NULL && data->allocated_size > 0)
		free(data);
	data->size = 0;
	data->allocated_size = 0;
	data->data = NULL;
}

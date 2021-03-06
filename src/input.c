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
bool			gather_full_input(t_ssl_data *input, char *path)
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
			return false;
		}
		gather_full_input_loop(input, fd);
		close(fd);
	}
	return true;
}

//------------------------------------------------------------------------------
void				data_soft_reset(t_ssl_data *data)
{
	if (data->data != NULL && data->allocated_size > 0)
		free(data->data);
	data->size = 0;
	data->allocated_size = 0;
	data->data = NULL;
}

//------------------------------------------------------------------------------
char				*get_data_as_str(t_ssl_data *data)
{
	char			*inputstr;
	if ((inputstr = malloc(data->size + 1)) == NULL)
	{
		ft_putstr("[Error] Bad malloc()\n");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(inputstr, data->data, data->size);
	inputstr[data->size] = '\0';
	if (data->size > 0 && inputstr[data->size - 1] == '\n')
		inputstr[data->size - 1] = '\0';
	return inputstr;
}

//------------------------------------------------------------------------------
t_ssl_data			*get_new_data_struct()
{
	t_ssl_data		*ret;
	if ((ret = (t_ssl_data*)malloc(sizeof(t_ssl_data))) == NULL)
	{
		ft_putstr("[Error] malloc() call failed in get_new_data()\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(ret, sizeof(t_ssl_data));
	return (ret);
}

//------------------------------------------------------------------------------
t_ssl_data			*copy_ssl_data(t_ssl_data *src)
{
	t_ssl_data		*ret = get_new_data_struct();

	if (ret == NULL)
		return NULL;
	if ((ret->data = malloc(src->size)) == NULL)
	{
		clean_data_struct(ret);
		return NULL;
	}
	ft_memcpy(ret->data, src->data, src->size);
	ret->size = src->size;
	ret->allocated_size = ret->size;

	return (ret);
}

//------------------------------------------------------------------------------
void						clean_data_struct(t_ssl_data *toclean)
{
	data_soft_reset(toclean);
	free(toclean);
}

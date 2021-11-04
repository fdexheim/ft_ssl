#include "../inc/libft.h"

void			**bootleg_calloc(size_t n_elements, size_t element_size)
{
	void		**ret;

	if ((ret = malloc(sizeof(void*) * (n_elements + 1))) == NULL)
		return NULL;
	ret[n_elements] = NULL;
	for (size_t i = 0; i < n_elements; i++)
	{
		if ((ret[i] = malloc(element_size)) == NULL)
		{
			ft_putstr("[Error] malloc call failed\n");
			return NULL;
		}
		ft_bzero(ret[i], element_size);
	}
	return (ret);
}

void			free_array(void **array)
{
	if (array == NULL)
		return ;
	for (size_t i = 0; array[i] != NULL; i++)
		free(array[i]);
	free(array);
}

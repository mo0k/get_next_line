#include "get_next_line.h"
#include "libft.h"

t_file	*check_fd(int fd, t_file *file)
{
	char *buff;

	buff = NULL;
	if (!file || fd < 0 || read(fd, buff, 0) == -1)
		return (NULL);
	while (file->next)
	{
		if (file->next->fd == fd)
			return (file->next);
		file = file->next;
	}
	if(!(file->next = (t_file*)malloc(sizeof(t_file))))
		return (NULL);
	file->next->next = NULL;
	file->next->fd = fd;
	file->next->data = NULL;
	file->next->read = 0;
	file->next->all_read = 0;
	file->next->state = 0;

	return (file->next);
}

int					stock_line(t_file *file, char *buff)
{
	char			*tmp;

	if (!file || !buff)
		return (-1);
	if (file->read)
	{
		tmp = ft_strjoin(file->data, buff);
		free(file->data);
		file->data = ft_strdup(tmp);
		free(tmp);
	}
	else
	{
		file->data = ft_strdup(buff);
		file->read = 1;
	}

	return (1);
}

int					read_line(t_file *file, char **line)
{
	char			buff[BUFF_SIZE + 1];
	int				ret;
	char			*p;

	if (!file || !line)
		return (ERROR);

	while ((ret = read(file->fd, buff, BUFF_SIZE)) > 0 || (file->state && file->data[0]))
	{
		file->state = 1;
		buff[ret] = 0;
		if (stock_line(file, buff) == -1)
			return (-1);
		p = ft_strchr(file->data, 0x0a);
		if (!p) //NON trouver 0x0a
		{
			if (!ret && file->read)
			{
				line[0] = ft_strdup(file->data);
				free(file->data);
				file->all_read = 1;
				return (END_OF_LINE);
			}
		}
		else //0x0a trouver
		{
			line[0] = (char*)ft_memalloc(p - file->data + 1);
			if (p - file->data > 0)
				line[0] = (char*)ft_memmove(line[0], file->data, p - file->data);
			free(file->data);
			file->data = ft_strdup(p + 1);
			return (END_OF_LINE);
		}
	}
	return (END_OF_FILE);
}

int					get_next_line(const int fd, char **line)
{
	static t_file	*file;
	t_file			*tmp;
	int 			ret;

	if(!line)
		return (ERROR);
	if(!(line[0] = (char*)ft_memalloc(1)))
			return (ERROR);
	if(!file)
	{
		if(!(file = (t_file*)malloc(sizeof(t_file))))
			return (ERROR);
		file->next = NULL;
	}
	tmp = file;
	if (!(file = check_fd(fd, file)))
		return (ERROR);
	if ((ret = read_line(file, &line[0])))
	{
		file = tmp;
		return (ret);
	}
	return (ret);
}

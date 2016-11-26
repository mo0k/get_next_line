/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmoucade <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 06:38:16 by jmoucade          #+#    #+#             */
/*   Updated: 2016/11/26 06:38:20 by jmoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static t_file		*check_fd(int fd, t_file *f)
{
	char			*buff;

	buff = NULL;
	if (!f || fd < 0 || read(fd, buff, 0) == -1)
		return (NULL);
	while (f->next)
	{
		if (f->next->fd == fd)
			return (f->next);
		f = f->next;
	}
	if (!(f->next = (t_file*)malloc(sizeof(t_file))))
		return (NULL);
	f->next->next = NULL;
	f->next->fd = fd;
	f->next->data = NULL;
	f->next->read = 0;
	f->next->all_read = 0;
	f->next->protect = 0;
	return (f->next);
}

static int			realloc_line(t_file *f, char *buff)
{
	char			*tmp;

	if (!f || !buff)
		return (0);
	if (f->read)
	{
		tmp = ft_strjoin(f->data, buff);
		free(f->data);
		f->data = ft_strdup(tmp);
		free(tmp);
	}
	else
	{
		f->data = ft_strdup(buff);
		f->read = 1;
	}
	return (1);
}

static int			read_line(t_file *f, char **line)
{
	char			buff[BUFF_SIZE + 1];
	int				ret;
	char			*p;

	if (!f || !line)
		return (ERROR);
	while ((ret = read(f->fd, buff, BUFF_SIZE)) || (f->protect && f->data[0]))
	{
		buff[ret] = 0;
		f->protect = 1;
		if (!realloc_line(f, buff))
			return (ERROR);
		p = ft_strchr(f->data, 0x0a);
		if (!p)
		{
			if (!ret && f->read)
			{
				line[0] = ft_strdup(f->data);
				free(f->data);
				return (END_OF_LINE);
			}
		}
		else
		{
			line[0] = (char*)ft_memalloc(p - f->data + 1);
			if (p - f->data > 0)
				line[0] = (char*)ft_memmove(line[0], f->data, p - f->data);
			free(f->data);
			f->data = ft_strdup(p + 1);
			return (END_OF_LINE);
		}
	}
	return (END_OF_FILE);
}

int					get_next_line(const int fd, char **line)
{
	static t_file	*f;
	t_file			*tmp;
	int				ret;

	if (!line)
		return (ERROR);
	if (!(line[0] = (char*)ft_memalloc(1)))
		return (ERROR);
	if (!f)
	{
		if (!(f = (t_file*)malloc(sizeof(t_file))))
			return (ERROR);
		f->next = NULL;
	}
	tmp = f;
	if (!(f = check_fd(fd, f)))
		return (ERROR);
	if ((ret = read_line(f, &line[0])))
	{
		f = tmp;
		return (END_OF_LINE);
	}
	return (END_OF_FILE);
}

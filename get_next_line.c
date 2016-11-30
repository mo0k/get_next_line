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

static int			memcat(char **dst, char *src)
{
	char			*tmp;

	if (!dst || !src || !*dst)
		return (0);
	if (!(tmp = ft_strjoin(*dst, src)))
		return (0);
	free(*dst);
	if (!(*dst = ft_strdup(tmp)))
		return (0);
	free(tmp);
	return (1);
}

static int			is_endline(t_file *f, char **line, int ret)
{
	char			*p;

	if ((p = ft_strchr(f->data, 0x0a)))
	{
		free(line[0]);
		if (!(line[0] = (char*)ft_memalloc(p - f->data + 1)))
			return (ERROR);
		line[0] = (char*)ft_memmove(line[0], f->data, p - f->data);
		free(f->data);
		if (!(f->data = ft_strdup(p + 1)))
			return (ERROR);
		return (END_OF_LINE);
	}
	if (!p && !ret && f->read)
	{
		free(line[0]);
		if (!(line[0] = ft_strdup(f->data)))
			return (ERROR);
		ft_bzero(f->data, ft_strlen(f->data));
		free(f->data);
		return (END_OF_LINE);
	}
	return (0);
}

static int			read_line(t_file *f, char **line)
{
	char			buff[BUFF_SIZE + 1];
	int				ret;
	int				endline;

	if (!f || !line || !*line)
		return (ERROR);
	while ((ret = read(f->fd, buff, BUFF_SIZE)) || (f->protect && f->data[0]))
	{
		buff[ret] = 0;
		f->protect = 1;
		if (!f->read)
		{
			if (!(f->data = ft_strdup(buff)))
				return (ERROR);
			f->read = 1;
		}
		else if (!memcat(&f->data, buff))
			return (ERROR);
		if ((endline = is_endline(f, line, ret)))
			return (END_OF_LINE);
		if (endline == ERROR)
			return (ERROR);
	}
	return (END_OF_FILE);
}

int					get_next_line(const int fd, char **line)
{
	static t_file	*f;
	t_file			*tmp;
	int				ret;

	if (!line || !(line[0] = (char*)ft_memalloc(1)))
		return (ERROR);
	if (!f)
	{
		if (!(f = (t_file*)malloc(sizeof(t_file))))
			return (ERROR);
		f->next = NULL;
	}
	tmp = f;
	if (!(f = check_fd(fd, f)))
	{
		f = tmp;
		return (ERROR);
	}
	if ((ret = read_line(f, line)))
	{
		f = tmp;
		return (END_OF_LINE);
	}
	return ((ret == ERROR) ? ERROR : END_OF_FILE);
}

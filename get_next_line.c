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
#include <stdio.h>

static t_file		*check_fd(int fd, t_file **f)
{
	char			*buff;
	t_file			*new;

	buff = NULL;
	if (fd < 0 || read(fd, buff, 0) == -1)
		return (NULL);
	if (!*f)
	{
		if (!(new = (t_file*)malloc(sizeof(t_file))))
			return (NULL);
		new->fd = fd;
		new->data = NULL;
		new->eof = 0;
		new->next = NULL;
		*f = new;
		return (new);
	}
	new = *f;
	if (!new)
		return (NULL);
	if (new->fd == fd)
		return (new);
	return (check_fd(fd, &new->next));
}

static int			ft_stracat(char **dst, char *src)
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
	char			*r;
	char 			*tmp;

	if ((r = ft_strchr(f->data, 0x0a)))
	{
		*r = 0;
		if (!(tmp = ft_strdup(r + 1)) || !(*line = ft_strdup(f->data)))
			return (ERROR);
		free(f->data);
		if (!(f->data = ft_strdup(tmp)))
			return (ERROR);
		free(tmp);
		return (END_OF_LINE);
	}
	else if (ret == 0 && (int)ft_strlen(f->data) == 0)
			f->eof = 1;
	else if(ret == 0 && (int)ft_strlen(f->data) > 0)
	{
			if (!(*line = ft_strdup(f->data)))
				return (ERROR);
			free(f->data);
			f->eof = 1;
			return (END_OF_LINE);
	}
	return (0);
}

static int			read_line(t_file *f, char **line, char *buff)
{
	int				ret;
	int 			endline;

	if (!f) 
		return (ERROR);
	while ((ret = read(f->fd, buff, BUFF_SIZE)) || f->eof == 0)
	{
		buff[ret] = 0;
		if (!f->data)
		{
			if (!(f->data = ft_strdup(buff)))
				return (ERROR);
		}
		else if (ret > 0)
		{
			if (!ft_stracat(&f->data, buff))
				return (ERROR);
		}
		if ((endline = is_endline(f, line, ret)) == END_OF_LINE)
			return (END_OF_LINE);
		else if (endline == ERROR)
			return (ERROR);
	}
	return (END_OF_FILE);
}

int					get_next_line(const int fd, char **line)
{
	static t_file	*f;
	t_file			*tmp;
	int				ret;
	char			*buff;

	if (!(buff = (char*)ft_memalloc(sizeof(char) * BUFF_SIZE + 1)))
			return (ERROR);
	if (!(tmp = check_fd(fd, &f)))
		return (ERROR);
	ret = read_line(tmp, line, buff);
	free(buff);
	if (ret == END_OF_LINE)
		return (END_OF_LINE);
	else if (ret == ERROR)
		return (ERROR);
	else
		return (END_OF_FILE);
}

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

static t_file		*check_fd(int fd, t_file *f)
{
	//printf("\tSTART CHECK_FD:%d\n", fd);
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

static int			ft_stracat(char **dst, char *src)
{
	//printf("\t\tSTART FT_STRACAT\n");
	char			*tmp;

	if (!dst || !src || !*dst)
		return (0);
	//printf("\t\t\tAvant strjoin => *dst:\n%s, src:\n%s\n", *dst, src);
	if (!(tmp = ft_strjoin(*dst, src)))
		return (0);
	free(*dst);
	if (!(*dst = ft_strdup(tmp)))
		return (0);
	//printf("\t\t\tApres strdup => *dst:\n%s\n", *dst);
	free(tmp);
	return (1);
}

static int			is_endline(t_file *f, char **line, int ret)
{
	printf("\t\tSTART IS_ENDLINE\n");
	char			*r;

	if ((r = ft_strchr(f->data, 0x0a)))
	{
		//printf("\t\t\t0x0a trouver => r:\n%s\n", r);
		//printf("\t\t\tVERIFICATION => longeur r:%d\n", (int)ft_strlen(r));
		free(line[0]);
		if (!(line[0] = (char*)ft_memalloc(r - f->data + 1)))
			return (ERROR);
		line[0] = ft_memmove(line[0], f->data, r - f->data);
		free(f->data);
		//printf("\t\t\tVERIFICATION => r:\n%s\n", r);

		//printf("\t\t\tVERIFICATION => longeur r + 1:%d\n", (int)ft_strlen(r+1));
		if (!(f->data = (char*)ft_memalloc(ft_strlen(r))))
			return (ERROR);
		f->data = ft_memmove(f->data, r + 1, ft_strlen(r + 1));
		//printf("\t\t\tVERIFICATION => longeur f->data:%d\n", (int)ft_strlen(f->data));
		//printf("\t\t\tApres modif => f->data:%s\n", f->data);
		return (END_OF_LINE);
	}
	if (!r && !ret && f->read)
	{
		//printf("\t\t\tLast line of file\n");
		if ((int)ft_strlen(f->data) == 0){
			f->protect = 0;
			return (END_OF_FILE);
		}
		free(line[0]);
		//printf("\t\t\tVERIFICATION => longeur f->data:%d\n", (int)ft_strlen(f->data));
		if (!(line[0] = ft_strdup(f->data)))
			return (ERROR);
		//printf("\t\t\tVERIFICATION => longeur *line:%d\n", (int)ft_strlen(*line));
		free(f->data);
		f->protect = 0;
		return (END_OF_LINE);
	}
	return (0);
}

static int			read_line(t_file *f, char **line)
{
	//printf("\tSTART READ_LINE\n");
	char			buff[BUFF_SIZE + 1];
	int				ret;
	int				endline;

	if (!f || !line || !*line)
		return (ERROR);
	while ((ret = read(f->fd, buff, BUFF_SIZE)) || (f->protect))// && f->data[0]))
	{
		//printf("\t\tIN READ => ret:%d, buff[%d]=\n%s\n", ret, BUFF_SIZE, buff);
		buff[ret] = 0;
		f->protect = 1;
		if (!f->read)
		{
			//printf("\t\tPremiere allocation de f->data\n");
			if (!(f->data = ft_strdup(buff)))
				return (ERROR);
			f->read = 1;
			//printf("\t\tf->data:\n%s\n", f->data);
		}
		else if (!ft_stracat(&f->data, buff)) //AJOUTER RET > 0
			return (ERROR);
		//printf("\t\tAvant is_endline\n");
		if ((endline = is_endline(f, line, ret)))
		{
			//printf("\t\tendline=%d", endline);// line:\n%s\n", endline, *line);
			return (END_OF_LINE);
		}
		if (endline == ERROR)
		{
			//printf("\t\tendline error\n");
			return (ERROR);
		}
		//printf("\t\tFin de line no found\n");
		//TERTER ft_bzero(buff);
	}
	return (END_OF_FILE);
}

int					get_next_line(const int fd, char **line)
{
	//printf("START GNL\n");
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
	//printf("\t\tFD:%d\n", f->fd);
	if ((ret = read_line(f, line)))
	{
		f = tmp;
		//printf("ret:%d, line:%s\n", ret, *line);
		return (END_OF_LINE);
	}
	//printf("ret end:%d, line:%s\n", ret, *line);
	return ((ret == ERROR) ? ERROR : END_OF_FILE);
}

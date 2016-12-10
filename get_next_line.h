/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmoucade <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/26 06:38:24 by jmoucade          #+#    #+#             */
/*   Updated: 2016/11/26 06:38:26 by jmoucade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdlib.h>

# define ERROR 		-1
# define END_OF_FILE 0
# define END_OF_LINE 1

# define BUFF_SIZE 8

typedef struct		s_file
{
	int				fd;
	char			*data;
	int				eof;
	struct s_file	*next;
}					t_file;

int					get_next_line(const int fd, char **line);

#endif

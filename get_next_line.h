#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

//# define ERROR ft_putstr("Error");
# define USAGE ft_putstr("Usage: ./a.out target_file");

# define ERROR 		-1
# define END_OF_FILE 0
# define END_OF_LINE 1

# define BUFF_SIZE 10

typedef struct		s_file
{
	int 			fd;
	char 			*data;
	int 			read;
	int 			all_read;
	int 			state;
	struct s_file 	*next;
}					t_file;

t_file	*check_fd(int fd, t_file *file);
int 	read_line(t_file *file, char **line);
int 	stock_line(t_file *file, char *buff);
int		get_next_line(const int fd, char **line);

#endif

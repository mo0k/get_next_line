# get_next_line
This function read line from file descriptor.<br/><br/>
<b>Requirememt:</b> libft<br/><br/>
<b>Prototype:</b><br/>
          int  get_next_line(const int fd, char **line)<br/>

<b>Return:</b>  
          &nbsp;1   => read line<br/>
          &nbsp;0   => end of file<br/>
         -1   => error<br/>

<b>Usage:</b><br/>
        &nbsp;&nbsp;&nbsp;while (get_next_line(fd, &line) == 1)<br/>
        &nbsp;&nbsp;&nbsp;{<br/>
          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ft_putstr(line);<br/>
          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ft_putchar('\n');<br/>
          &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;free(line);<br/>
        &nbsp;&nbsp;&nbsp;}<br/>

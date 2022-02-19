#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# define MAX_INT 2147483647
# define BUFFER_SIZE 1024
int		check(int fd, char **line, size_t bufsize);
size_t	ft_strlen(const char *str);
void	*ft_memchr(const void *ptr, int c, size_t n);
char	*realloc_concat(char *s1, char *buf, size_t bufsize);
int		read_file(int fd, char **line, char *buf, char **remainder);
int		get_next_line(int fd, char **line);
#endif
#include "./gnl.hpp"

char	*realloc_concat(char *s1, char *buf, size_t bufsize)
{
	char	*result;
	size_t	len;
	size_t	i;

	len = (s1 ? ft_strlen(s1) : 0);
	if (!(result = (char *)malloc(len + bufsize + 1)))
		return (0);
	i = 0;
	if (s1)
	{
		while (i < len)
		{
			result[i] = s1[i];
			i++;
		}
		free(s1);
	}
	while (i < bufsize + len)
	{
		result[i] = buf[i - len];
		i++;
	}
	result[bufsize + len] = '\0';
	return (result);
}

int		check(int fd, char **line, size_t bufsize)
{
	if (fd < 0 || bufsize < 1 || bufsize > MAX_INT || !line)
		return (1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;

	i = 0;
	p = (unsigned char *)ptr;
	while (i < n)
	{
		if (p[i] == (unsigned char)c)
			return ((void *)&p[i]);
		i++;
	}
	return (0);
}

int		read_file(int fd, char **line, char *buf, char **rem)
{
	char	*endl;
	int		read_b;
	int		rem_bytes;

	while ((read_b = read(fd, buf, BUFFER_SIZE)))
	{
		if (read_b < 0)
			return (-1);
		if ((endl = ft_memchr(buf, '\n', read_b)))
		{
			*endl = '\0';
			*line = realloc_concat(*line, buf, ft_strlen(buf));
			rem_bytes = read_b - ft_strlen(buf) - 1;
			*rem = realloc_concat(*rem, endl + 1, rem_bytes);
			free(buf);
			return (1);
		}
		else
			*line = realloc_concat(*line, buf, read_b);
	}
	free(buf);
	return (0);
}
int		get_next_line(int fd, char **line)
{
	static char		*remainder;
	char			*buf;
	char			*endl;
	char			*tmp;

	if (check(fd, line, BUFFER_SIZE) || !(buf = (char *)malloc(BUFFER_SIZE)))
		return (-1);
	*line = realloc_concat(NULL, "", 0);
	if (remainder)
	{
		endl = ft_memchr(remainder, '\n', ft_strlen(remainder));
		if (endl)
			*endl = '\0';
		*line = realloc_concat(*line, remainder, ft_strlen(remainder));
		tmp = (endl ? realloc_concat(NULL, endl + 1, ft_strlen(endl + 1)) : 0);
		free(remainder);
		remainder = tmp;
		if (endl)
		{
			free(buf);
			return (1);
		}
	}
	return (read_file(fd, line, buf, &remainder));
}
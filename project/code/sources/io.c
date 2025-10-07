#include "ft_ssl.h"

static int grow(uint8_t **buf, size_t cur, size_t need, size_t *cap)
{
	size_t ncap = *cap ? *cap : 8192;
	while (cur + need > ncap)
		ncap *= 2;
	if (ncap == *cap)
		return (0);
	uint8_t *tmp = (uint8_t *)malloc(ncap);
	if (!tmp)
		return (-1);
	if (*buf && cur)
		ft_memcpy(tmp, *buf, cur);
	if (*buf)
		free(*buf);
	*buf = tmp;
	*cap = ncap;
	return (0);
}

int io_read_fd(int fd, uint8_t **out, size_t *len)
{
	uint8_t *dst = NULL;
	size_t cap = 0, w = 0;
	uint8_t buf[4096];
	ssize_t n;

	*out = NULL;
	*len = 0;
	if (grow(&dst, 0, 8192, &cap) < 0)
		return (-1);
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		if (grow(&dst, w, (size_t)n, &cap) < 0)
		{
			free(dst);
			return (-1);
		}
		ft_memcpy(dst + w, buf, n);
		w += (size_t)n;
	}
	if (n < 0)
	{
		free(dst);
		return (-1);
	}
	*out = dst;
	*len = w;
	return (0);
}

int io_read_file(const char *path, uint8_t **out, size_t *len)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	int r = io_read_fd(fd, out, len);
	close(fd);
	return (r);
}

int io_read_stdin(uint8_t **out, size_t *len)
{
	return (io_read_fd(STDIN_FILENO, out, len));
}

#include "ft_ssl.h"

int	read_all_stdin(uint8_t **out, size_t *len)
{
	uint8_t	buf[4096];
	uint8_t	*dst;
	size_t	cap;
	size_t	w;
	ssize_t	n;

	*out = NULL;
	*len = 0;
	cap = 8192;
	dst = (uint8_t *)malloc(cap);
	if (!dst)
		return (-1);
	w = 0;
	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
	{
		if (w + (size_t)n > cap)
		{
			size_t ncap = cap * 2;
			while (w + (size_t)n > ncap) ncap *= 2;
			uint8_t *tmp = (uint8_t *)malloc(ncap);
			if (!tmp) { free(dst); return (-1); }
			ft_memcpy(tmp, dst, w);
			free(dst);
			dst = tmp;
			cap = ncap;
		}
		ft_memcpy(dst + w, buf, n);
		w += (size_t)n;
	}
	if (n < 0) { free(dst); return (-1); }
	*out = dst;
	*len = w;
	return (0);
}

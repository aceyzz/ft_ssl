#include "ft_ssl.h"

static int	read_entire_file(const char *path, uint8_t **out, size_t *len)
{
	int		fd;
	ssize_t	n;
	uint8_t	buf[4096];
	uint8_t	*dst;
	size_t	cap;
	size_t	w;

	*out = NULL; *len = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	cap = 8192;
	dst = (uint8_t *)malloc(cap);
	if (!dst)
		return (close(fd), -1);
	w = 0;
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		if (w + (size_t)n > cap)
		{
			size_t ncap = cap * 2;
			while (w + (size_t)n > ncap) ncap *= 2;
			uint8_t *tmp = (uint8_t *)malloc(ncap);
			if (!tmp) { free(dst); close(fd); return (-1); }
			ft_memcpy(tmp, dst, w);
			free(dst);
			dst = tmp;
			cap = ncap;
		}
		ft_memcpy(dst + w, buf, n);
		w += (size_t)n;
	}
	close(fd);
	if (n < 0)
		free(dst);
	*out = dst; *len = w;
	return (0);
}

int	run_tasks(t_algo algo, t_flags flags, t_task *tasks)
{
	t_task		*t = tasks;
	uint8_t		*file_data;
	size_t		file_len;

	while (t)
	{
		if (t->kind == IN_FILE)
		{
			file_data = NULL; file_len = 0;
			if (read_entire_file(t->label, &file_data, &file_len) == 0)
			{
				t_task tmp; tmp.kind = IN_FILE; tmp.label = t->label; tmp.data = file_data; tmp.len = file_len; tmp.next = NULL;
				hash_and_print(algo, flags, &tmp);
				free(file_data);
			}
			else
				log_err3(STDERR_FILENO, "ft_ssl: ", t->label, ": No such file or directory");
		}
		else
			hash_and_print(algo, flags, t);
		t = t->next;
	}
	return (0);
}

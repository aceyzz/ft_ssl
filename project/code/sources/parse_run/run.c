#include "ft_ssl.h"

int	ensure_task_data(t_task *t)
{
	if (t->kind == IN_FILE && t->label)
	{
		int fd = open(t->label, O_RDONLY);
		if (fd < 0) return (-1);
		close(fd);
	}
	else if (t->kind == IN_STDIN || t->kind == IN_STRING)
	{
		if (!t->data || !t->len) return (-1);
	}
	return (0);
}

int	run_tasks(const t_algo_api *api, t_flags flags, t_task *tasks)
{
	for (t_task *t = tasks; t; t = t->next)
	{
		if (ensure_task_data(t) != 0)
			log_err3(STDERR_FILENO, "ft_ssl: ", t->label ? t->label : (char *)"", ": No such file or directory");
		else
			hash_and_print(api, flags, t);
	}
	return (0);
}

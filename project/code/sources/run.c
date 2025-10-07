#include "ft_ssl.h"

int	ensure_task_data(t_task *t)
{
	if (t->data && t->len)
		return (0);
	if (t->kind == IN_FILE && t->label)
	{
		uint8_t *buf = NULL; size_t n = 0;
		if (io_read_file(t->label, &buf, &n) != 0)
			return (-1);
		t->data = buf; t->len = n;
		return (0);
	}
	if ((t->kind == IN_STDIN || t->kind == IN_STRING) && (!t->data || !t->len))
		return (-1);
	return (0);
}

int	run_tasks(t_algo algo, t_flags flags, t_task *tasks)
{
	t_task *t = tasks;

	while (t)
	{
		if (ensure_task_data(t) != 0)
			log_err3(STDERR_FILENO, "ft_ssl: ", t->label ? t->label : (char *)"", ": No such file or directory");
		else
			hash_and_print(algo, flags, t);
		t = t->next;
	}
	return (0);
}

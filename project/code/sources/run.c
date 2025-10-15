#include "ft_ssl.h"

// sanity check avant
int ensure_task_data(t_task *t)
{
	if (t->kind == IN_FILE && t->label)
	{
		int fd = open(t->label, O_RDONLY);
		if (fd < 0)
			return (-1);
		close(fd);
		return (0);
	}
	if ((t->kind == IN_STDIN || t->kind == IN_STRING))
	{
		if (!t->data || !t->len)
			return (-1);
		return (0);
	}
	return (0);
}

// pour chaque task -> exec
int run_tasks(t_algo algo, t_flags flags, t_task *tasks)
{
	t_task *t = tasks;

	while (t)
	{
		if (ensure_task_data(t) != 0)
			log_err3(STDERR_FILENO, "ft_ssl: ", t->label ? t->label : (char *)"", ": No such file or directory"); // non bloquant
		else
			hash_and_print(algo, flags, t);
		t = t->next;
	}
	return (0);
}

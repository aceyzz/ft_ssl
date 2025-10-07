#include "ft_ssl.h"

int main(int argc, char **argv)
{
	t_algo	algo;
	t_flags	flags;
	t_task	*tasks;

	if (argc < 2)
		die_err("usage: ft_ssl command [command opts] [command args]", EXIT_FAILURE);
	tasks = NULL;
	parse_cli(argc, argv, &algo, &flags, &tasks);
	if (run_tasks(algo, flags, tasks) != 0)
	{
		task_clear(&tasks);
		exit(EXIT_FAILURE);
	}
	task_clear(&tasks);
	return (0);
}

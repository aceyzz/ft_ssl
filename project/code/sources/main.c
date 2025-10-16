#include "ft_ssl.h"

int	main(int argc, char **argv)
{
	const t_algo_api	*api;
	t_flags				flags;
	t_task				*tasks;

	if (argc < 2)
		algo_print_usage_and_die();

	tasks = NULL;
	parse_cli(argc, argv, &api, &flags, &tasks);
	if (run_tasks(api, flags, tasks) != 0)
	{
		task_clear(&tasks);
		return (EXIT_FAILURE);
	}
	task_clear(&tasks);
	return (0);
}

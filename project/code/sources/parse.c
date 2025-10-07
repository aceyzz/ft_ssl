#include "ft_ssl.h"

static int	is_cmd(const char *s, t_algo *algo)
{
	if (ft_strcmp(s, "md5") == 0) return (*algo = ALG_MD5, 1);
	if (ft_strcmp(s, "sha256") == 0) return (*algo = ALG_SHA256, 1);
	return (0);
}

static void	parse_short_flags(const char *arg, int *i, int argc, char **argv,
	t_flags *f, t_task **tasks, uint8_t **stdin_cache, size_t *stdin_len)
{
	int	k = 1;

	while (arg[k])
	{
		if (arg[k] == 'p')
		{
			f->p = 1;
			if (!*stdin_cache && io_read_stdin(stdin_cache, stdin_len) != 0)
				exit(EXIT_FAILURE);
			task_push_back(tasks, IN_STDIN, NULL, *stdin_cache, *stdin_len);
			k++;
		}
		else if (arg[k] == 'q')
			(f->q = 1, k++);
		else if (arg[k] == 'r')
			(f->r = 1, k++);
		else if (arg[k] == 's')
		{
			if (arg[k + 1] != '\0')
			{
				const char *s = arg + k + 1;
				task_push_back(tasks, IN_STRING, s, (const uint8_t *)s, ft_strlen(s));
				return ;
			}
			if (*i + 1 >= argc) die_err("ft_ssl: option requires an argument -- s", EXIT_FAILURE);
			*i += 1;
			task_push_back(tasks, IN_STRING, argv[*i], (const uint8_t *)argv[*i], ft_strlen(argv[*i]));
			return ;
		}
		else
		{
			char suf[2]; suf[0] = arg[k]; suf[1] = '\0';
			log_err2(STDERR_FILENO, "ft_ssl: illegal option -- ", suf);
			exit(EXIT_FAILURE);
		}
	}
}

void	parse_cli(int argc, char **argv, t_algo *algo, t_flags *flags, t_task **tasks)
{
	int			i = 2;
	int			seen_nonopt = 0;
	uint8_t		*stdin_cache = NULL;
	size_t		stdin_len = 0;

	if (!is_cmd(argv[1], algo))
		die_err("ft_ssl: Error: Unknown command.", EXIT_FAILURE);
	flags->p = 0; flags->q = 0; flags->r = 0;
	*tasks = NULL;

	while (i < argc)
	{
		if (!seen_nonopt && argv[i][0] == '-' && argv[i][1] != '\0')
			parse_short_flags(argv[i], &i, argc, argv, flags, tasks, &stdin_cache, &stdin_len);
		else
		{
			seen_nonopt = 1;
			task_push_back(tasks, IN_FILE, argv[i], NULL, 0);
		}
		i++;
	}
	if (*tasks == NULL)
	{
		if (!stdin_cache && io_read_stdin(&stdin_cache, &stdin_len) != 0) exit(EXIT_FAILURE);
		task_push_back(tasks, IN_STDIN, NULL, stdin_cache, stdin_len);
	}
	if (stdin_cache) free(stdin_cache);
}

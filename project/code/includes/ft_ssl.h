#ifndef FT_SSL_H
# define FT_SSL_H

// std
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdint.h>
# include <errno.h>

// custom
# include "colors.h"
# include "libft.h"
# include "md5.h"
# include "sha256.h"

// bonus
# include "sha512.h"

typedef struct s_flags
{
	int p;
	int q;
	int r;
}	t_flags;

typedef enum e_ikind
{
	IN_STDIN,
	IN_STRING,
	IN_FILE
}	t_ikind;

typedef struct s_task
{
	t_ikind			kind;
	char			*label;
	uint8_t			*data;
	size_t			len;
	int				from_p;
	struct s_task	*next;
}	t_task;

typedef char *(*t_hash_hex_fn)(const uint8_t *data, size_t len);
typedef char *(*t_hash_fd_hex_fn)(int fd);

typedef struct s_algo_api
{
	const char			*cmd;
	const char			*name;
	size_t				out_hex_len;
	t_hash_hex_fn		hash_hex;
	t_hash_fd_hex_fn	hash_fd_hex;
}	t_algo_api;

// registry
const t_algo_api	*algo_by_cmd(const char *cmd);
void				algo_print_usage_and_die(void);

// utils
char	*bin_to_hex(const uint8_t *buf, size_t n);

// parse et run
void	parse_cli(int argc, char **argv, const t_algo_api **api, t_flags *flags, t_task **tasks);
int		run_tasks(const t_algo_api *api, t_flags flags, t_task *tasks);
void	hash_and_print(const t_algo_api *api, t_flags flags, t_task *t);
int		ensure_task_data(t_task *t);

// tasks
void	task_push_back(t_task **head, t_ikind kind, const char *label, const uint8_t *data, size_t len, int from_p);
void	task_clear(t_task **head);

// i/o
int		io_read_fd(int fd, uint8_t **out, size_t *len);
int		io_read_file(const char *path, uint8_t **out, size_t *len);
int		io_read_stdin(uint8_t **out, size_t *len);

// logs
void	log_ok(int fd, char *msg);
void	log_warn(int fd, char *msg);
void	log_err(int fd, char *msg);
void	log_err2(int fd, char *m1, char *m2);
void	log_err3(int fd, char *m1, char *m2, char *m3);
// exits
void	die_ok(char *msg);
void	die_err(char *msg, int code);

#endif

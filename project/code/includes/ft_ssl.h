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
# include "io.h"

// type d'algo 
typedef enum e_algo {
	ALG_MD5,
	ALG_SHA256
} t_algo;

// type d'input
typedef enum e_ikind {
	IN_STDIN,
	IN_STRING,
	IN_FILE
} t_ikind;

# include "digest.h"

// q = quiet, r = reverse, p = print stdin sur stdout et ajouter le checksum à stdout
typedef struct s_flags {
	int p;
	int q;
	int r;
} t_flags;

// 1 task = 1 commande
typedef struct s_task
{
	t_ikind			kind;
	char			*label;
	uint8_t			*data;
	size_t			len;
	int				from_p;
	struct s_task	*next;
}	t_task;

void	parse_cli(int argc, char **argv, t_algo *algo, t_flags *flags, t_task **tasks);
int		run_tasks(t_algo algo, t_flags flags, t_task *tasks);
void	task_push_back(t_task **head, t_ikind kind, const char *label, const uint8_t *data, size_t len, int from_p);
void	task_clear(t_task **head);
void	hash_and_print(t_algo algo, t_flags flags, t_task *t);
int		ensure_task_data(t_task *t);

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

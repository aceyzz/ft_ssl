#include "ft_ssl.h"

// pointeur sur fonction (plus de if/else) : seule ligne a modifier pour ajouter un algo
static const t_algo_api g_algos[] = {
	{ "md5",    "MD5",    32, md5_hex,    md5_fd_hex    },
	{ "sha256", "SHA2-256", 64, sha256_hex, sha256_fd_hex },
	{ "sha512", "SHA2-512", 128, sha512_hex, sha512_fd_hex },
};
static const size_t g_algos_count = sizeof(g_algos) / sizeof(g_algos[0]);

// dispatcher (parcourt tab de pointeurs sur fonctions, plus de if/else)
const t_algo_api	*algo_by_cmd(const char *cmd)
{
	for (size_t i = 0; i < g_algos_count; ++i)
		if (ft_strcmp(g_algos[i].cmd, cmd) == 0)
			return (&g_algos[i]);
	return (NULL);
}

void	algo_print_usage_and_die(void)
{
	ft_putstr_fd("usage: ft_ssl command [flags] [file/string]\n\n", STDERR_FILENO);
	ft_putstr_fd("Commands:\n", STDERR_FILENO);
	for (size_t i = 0; i < g_algos_count; ++i)
	{
		ft_putstr_fd(g_algos[i].cmd, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
	ft_putstr_fd("\nFlags:\n-p -q -r -s\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

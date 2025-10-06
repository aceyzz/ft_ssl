#include "ft_ssl.h"

void	exit_msg(int fd, char *message, int exit_code)
{
	ft_putstr_fd(exit_code == EXIT_SUCCESS ? LIME : REDD, fd);
	ft_putstr_fd(message, fd);
	write(fd, "\n", 1);
	ft_putstr_fd(RST, fd);
	exit(exit_code);
}

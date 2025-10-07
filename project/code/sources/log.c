#include "ft_ssl.h"

static void	print_tag(int fd,  char *color,  char *tag)
{
	ft_putstr_fd(color, fd);
	ft_putstr_fd(tag, fd);
	ft_putstr_fd(RST, fd);
	ft_putstr_fd(" ", fd);
}

static void	print_line(int fd,  char *msg)
{
	ft_putstr_fd((char *)msg, fd);
	write(fd, "\n", 1);
}

void	log_ok(int fd,  char *msg)
{
	print_tag(fd, LIME, "[►]");
	print_line(fd, msg);
}

void	log_warn(int fd,  char *msg)
{
	print_tag(fd, GOLD, "[►]");
	print_line(fd, msg);
}

void	log_err(int fd,  char *msg)
{
	print_tag(fd, REDD, "[►]");
	print_line(fd, msg);
}

void	log_err2(int fd,  char *m1,  char *m2)
{
	print_tag(fd, REDD, "[►]");
	ft_putstr_fd(m1, fd);
	ft_putstr_fd(m2, fd);
	write(fd, "\n", 1);
}

void	log_err3(int fd,  char *m1,  char *m2,  char *m3)
{
	print_tag(fd, REDD, "[►]");
	ft_putstr_fd(m1, fd);
	ft_putstr_fd(m2, fd);
	ft_putstr_fd(m3, fd);
	write(fd, "\n", 1);
}

void	die_ok( char *msg)
{
	log_ok(STDOUT_FILENO, msg);
	exit(EXIT_SUCCESS);
}

void	die_err( char *msg, int code)
{
	log_err(STDERR_FILENO, msg);
	exit(code);
}

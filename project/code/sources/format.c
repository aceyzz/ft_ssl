#include "ft_ssl.h"

static void	print_line_named(char *name, char *label, char *hex)
{
	ft_putstr_fd(name, STDOUT_FILENO);
	ft_putstr_fd(" (", STDOUT_FILENO);
	ft_putstr_fd(label, STDOUT_FILENO);
	ft_putstr_fd(") = ", STDOUT_FILENO);
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_line_named_quoted(char *name, char *label, char *hex)
{
	ft_putstr_fd(name, STDOUT_FILENO);
	ft_putstr_fd(" (\"", STDOUT_FILENO);
	ft_putstr_fd(label, STDOUT_FILENO);
	ft_putstr_fd("\") = ", STDOUT_FILENO);
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_line_rev(char *hex, char *suffix)
{
	ft_putstr_fd(hex, STDOUT_FILENO);
	if (suffix && *suffix)
	{
		ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(suffix, STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_hash_line(t_algo algo, t_flags f, t_task *t, char *hex)
{
	char	*name;

	name = (algo == ALG_MD5) ? (char *)"MD5" : (char *)"SHA256";
	if (f.q)
	{
		ft_putstr_fd(hex, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	if (f.r)
	{
		if (t->kind == IN_FILE && t->label)
			return (print_line_rev(hex, t->label));
		if (t->kind == IN_STRING && t->label)
		{
			char *suf = ft_strjoin("\"", t->label);
			char *tmp = suf ? ft_strjoin(suf, "\"") : NULL;
			if (tmp) print_line_rev(hex, tmp);
			if (suf) free(suf);
			if (tmp) free(tmp);
			return ;
		}
		print_line_rev(hex, NULL);
		return ;
	}
	if (t->kind == IN_FILE && t->label)
		return (print_line_named(name, t->label, hex));
	if (t->kind == IN_STRING && t->label)
		return (print_line_named_quoted(name, t->label, hex));
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	hash_and_print(t_algo algo, t_flags flags, t_task *t)
{
	const t_algo_api *api = algo_api(algo);
	char *hex;

	if (t->kind == IN_STDIN && flags.p && t->data && t->len)
		write(STDOUT_FILENO, t->data, t->len);
	hex = api->hash_hex(t->data, t->len);
	if (!hex)
	{
		log_err(STDERR_FILENO, "ft_ssl: hash failed");
		return ;
	}
	print_hash_line(algo, flags, t, hex);
	free(hex);
}

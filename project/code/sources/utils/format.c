#include "ft_ssl.h"

// PRINTER UTILS (inchangés)

static void	print_line_named(char *name, char *label, char *hex)
{
	ft_putstr_fd(name, STDOUT_FILENO);
	ft_putstr_fd("(", STDOUT_FILENO);
	ft_putstr_fd(label, STDOUT_FILENO);
	ft_putstr_fd(")= ", STDOUT_FILENO);
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_line_named_quoted(char *name, char *label, char *hex)
{
	ft_putstr_fd(name, STDOUT_FILENO);
	ft_putstr_fd("(\"", STDOUT_FILENO);
	ft_putstr_fd(label, STDOUT_FILENO);
	ft_putstr_fd("\")=", STDOUT_FILENO);
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

static void	print_p_quoted_data(const uint8_t *data, size_t len, char *hex)
{
	size_t n = len;
	if (n && data[n - 1] == '\n') n--;
	ft_putstr_fd("(\"", STDOUT_FILENO);
	if (n) write(STDOUT_FILENO, data, n);
	ft_putstr_fd("\")= ", STDOUT_FILENO);
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_stdin_named(char *hex)
{
	ft_putstr_fd("(stdin)= ", STDOUT_FILENO);
	ft_putstr_fd(hex, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	print_hash_line(const t_algo_api *api, t_flags f, t_task *t, char *hex)
{
	char *name = (char *)api->name;

	if (t->kind == IN_STDIN)
	{
		if (f.q)
		{
			if (t->from_p && t->data && t->len)
				write(STDOUT_FILENO, t->data, t->len);
			ft_putstr_fd(hex, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
			return;
		}
		if (t->from_p) { print_p_quoted_data(t->data, t->len, hex); return; }
		if (f.r)       { print_line_rev(hex, NULL); return; }
		print_stdin_named(hex); return;
	}
	if (f.q) { ft_putstr_fd(hex, STDOUT_FILENO); ft_putchar_fd('\n', STDOUT_FILENO); return; }
	if (f.r)
	{
		if (t->kind == IN_FILE && t->label) { print_line_rev(hex, t->label); return; }
		if (t->kind == IN_STRING && t->label)
		{
			char *suf = ft_strjoin("\"", t->label);
			char *tmp = suf ? ft_strjoin(suf, "\"") : NULL;
			if (tmp) print_line_rev(hex, tmp);
			if (suf) free(suf);
			if (tmp) free(tmp);
			return;
		}
		print_line_rev(hex, NULL); return;
	}
	if (t->kind == IN_FILE && t->label)   { print_line_named(name, t->label, hex); return; }
	if (t->kind == IN_STRING && t->label) { print_line_named_quoted(name, t->label, hex); return; }
	ft_putstr_fd(hex, STDOUT_FILENO); ft_putchar_fd('\n', STDOUT_FILENO);
}

// orchestration du hash + print (via api)
void	hash_and_print(const t_algo_api *api, t_flags flags, t_task *t)
{
	char *hex = NULL;

	if (t->kind == IN_FILE && t->label)
	{
		int fd = open(t->label, O_RDONLY);
		if (fd < 0) { log_err3(STDERR_FILENO, "ft_ssl: ", t->label, ": No such file or directory"); return; }
		hex = api->hash_fd_hex(fd);
		close(fd);
	}
	else
		hex = api->hash_hex(t->data, t->len);

	if (!hex) { log_err(STDERR_FILENO, "ft_ssl: hash failed"); return; }
	print_hash_line(api, flags, t, hex);
	free(hex);
}

#include "ft_ssl.h"

static char *bin_to_hex(const uint8_t *buf, size_t n)
{
	static const char *h = "0123456789abcdef";
	char *out = (char *)malloc(n * 2 + 1);
	size_t i;

	if (!out)
		return (NULL);
	for (i = 0; i < n; i++)
	{
		out[i * 2] = h[(buf[i] >> 4) & 0xF];
		out[i * 2 + 1] = h[buf[i] & 0xF];
	}
	out[n * 2] = '\0';
	return (out);
}

char *md5_hex(const uint8_t *data, size_t len)
{
	t_md5_ctx ctx;
	uint8_t digest[16];

	md5_init(&ctx);
	if (data && len)
		md5_update(&ctx, data, len);
	md5_final(&ctx, digest);
	return (bin_to_hex(digest, 16));
}

char *sha256_hex(const uint8_t *data, size_t len)
{
	t_sha256_ctx ctx;
	uint8_t digest[32];
	sha256_init(&ctx);
	if (data && len)
		sha256_update(&ctx, data, len);
	sha256_final(&ctx, digest);
	return (bin_to_hex(digest, 32));
}

char *md5_fd_hex(int fd)
{
	t_md5_ctx ctx;
	uint8_t digest[16];
	uint8_t buf[4096];
	ssize_t n;

	md5_init(&ctx);
	while ((n = read(fd, buf, sizeof(buf))) > 0)
		md5_update(&ctx, buf, (size_t)n);
	if (n < 0)
		return (NULL);
	md5_final(&ctx, digest);
	return (bin_to_hex(digest, 16));
}

char *sha256_fd_hex(int fd)
{
	t_sha256_ctx ctx;
	uint8_t digest[32];
	uint8_t buf[4096];
	ssize_t n;

	sha256_init(&ctx);
	while ((n = read(fd, buf, sizeof(buf))) > 0)
		sha256_update(&ctx, buf, (size_t)n);
	if (n < 0)
		return (NULL);
	sha256_final(&ctx, digest);
	return (bin_to_hex(digest, 32));
}

// SI AJOUT ALGO
// instances statiques de t_algo_api pour chaque algo vers pointeurs de fonctions
static t_algo_api g_md5 = {"MD5", 32, md5_hex, md5_fd_hex};
static t_algo_api g_sha256 = {"SHA256", 64, sha256_hex, sha256_fd_hex};

// SI AJOUT ALGO
const t_algo_api *algo_api(t_algo a)
{
	if (a == ALG_MD5)
		return (&g_md5);
	return (&g_sha256);
}

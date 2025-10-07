#ifndef DIGEST_H
# define DIGEST_H

# include <stdint.h>

typedef enum e_algo t_algo;

typedef char *(*t_hash_hex_fn)(const uint8_t *data, size_t len);
typedef char *(*t_hash_fd_hex_fn)(int fd);

typedef struct s_algo_api
{
	const char			*name;
	size_t				out_hex_len;
	t_hash_hex_fn		hash_hex;
	t_hash_fd_hex_fn	hash_fd_hex;
}	t_algo_api;

const t_algo_api	*algo_api(t_algo a);

typedef struct s_md5_ctx
{
	uint32_t	a, b, c, d;
	uint64_t	total_len;
	uint8_t		buf[64];
	size_t		buf_len;
}	t_md5_ctx;

typedef struct s_sha256_ctx
{
	uint32_t	h[8];
	uint64_t	total_len;
	uint8_t		buf[64];
	size_t		buf_len;
}	t_sha256_ctx;

void	md5_init(t_md5_ctx *ctx);
void	md5_update(t_md5_ctx *ctx, const uint8_t *data, size_t len);
void	md5_final(t_md5_ctx *ctx, uint8_t out[16]);

void	sha256_init(t_sha256_ctx *ctx);
void	sha256_update(t_sha256_ctx *ctx, const uint8_t *data, size_t len);
void	sha256_final(t_sha256_ctx *ctx, uint8_t out[32]);

char	*md5_hex(const uint8_t *data, size_t len);
char	*sha256_hex(const uint8_t *data, size_t len);

char	*md5_fd_hex(int fd);
char	*sha256_fd_hex(int fd);

#endif

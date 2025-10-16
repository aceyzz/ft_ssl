#ifndef SHA256_H
# define SHA256_H

# include <stdint.h>
# include <stddef.h>

typedef struct s_sha256_ctx
{
	uint32_t	h[8];
	uint64_t	total_len;
	uint8_t		buf[64];
	size_t		buf_len;
}	t_sha256_ctx;

// core
void	sha256_init(t_sha256_ctx *ctx);
void	sha256_update(t_sha256_ctx *ctx, const uint8_t *data, size_t len);
void	sha256_final(t_sha256_ctx *ctx, uint8_t out[32]);

// wrappers
char	*sha256_hex(const uint8_t *data, size_t len);
char	*sha256_fd_hex(int fd);

#endif

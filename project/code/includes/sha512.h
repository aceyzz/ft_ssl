#ifndef SHA512_H
# define SHA512_H

# include <stdint.h>
# include <stddef.h>

typedef struct	s_sha512_ctx
{
	uint64_t	h[8];
	uint64_t	total_low;
	uint64_t	total_high;
	uint8_t 	buf[128];
	size_t		buf_len;
}	t_sha512_ctx;

// core
void	sha512_init(t_sha512_ctx *ctx);
void	sha512_update(t_sha512_ctx *ctx, const uint8_t *data, size_t len);
void	sha512_final(t_sha512_ctx *ctx, uint8_t out[64]);

// wrappers
char	*sha512_hex(const uint8_t *data, size_t len);
char	*sha512_fd_hex(int fd);

#endif

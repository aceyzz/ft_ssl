#ifndef MD5_H
# define MD5_H

# include <stdint.h>
# include <stddef.h>

typedef struct s_md5_ctx
{
	uint32_t	a, b, c, d;
	uint64_t	total_len;
	uint8_t		buf[64];
	size_t		buf_len;
}	t_md5_ctx;

// core
void	md5_init(t_md5_ctx *ctx);
void	md5_update(t_md5_ctx *ctx, const uint8_t *data, size_t len);
void	md5_final(t_md5_ctx *ctx, uint8_t out[16]);

// wrappers
char	*md5_hex(const uint8_t *data, size_t len);
char	*md5_fd_hex(int fd);

#endif

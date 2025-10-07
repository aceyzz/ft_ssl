#ifndef DIGEST_H
# define DIGEST_H

# include <stddef.h>
# include <stdint.h>

typedef enum e_algo t_algo;

typedef char *(*t_hash_hex_fn)(const uint8_t *data, size_t len);

typedef struct s_algo_api
{
	const char		*name;
	size_t			out_hex_len;
	t_hash_hex_fn	hash_hex;
}	t_algo_api;

const t_algo_api	*algo_api(t_algo a);

char	*md5_hex(const uint8_t *data, size_t len);
char	*sha256_hex(const uint8_t *data, size_t len);

#endif

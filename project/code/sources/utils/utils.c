#include "ft_ssl.h"

char	*bin_to_hex(const uint8_t *buf, size_t n)
{
	static const char *h = "0123456789abcdef";
	char *out = (char *)malloc(n * 2 + 1);
	if (!out)
		return (NULL);
	
	for (size_t i = 0; i < n; i++)
	{
		out[i * 2]     = h[(buf[i] >> 4) & 0xF];
		out[i * 2 + 1] = h[buf[i] & 0xF];
	}
	out[n * 2] = '\0';
	return (out);
}

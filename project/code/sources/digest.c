#include "ft_ssl.h"

static t_algo_api g_md5 = { "MD5", 32, md5_hex };
static t_algo_api g_sha256 = { "SHA256", 64, sha256_hex };

static char *bin_to_hex(const uint8_t *buf, size_t n)
{
    static const char *h = "0123456789abcdef";
    char *out = (char *)malloc(n * 2 + 1);
    size_t i;
    if (!out) return NULL;
    for (i = 0; i < n; i++)
    {
        out[i * 2] = h[(buf[i] >> 4) & 0xF];
        out[i * 2 + 1] = h[buf[i] & 0xF];
    }
    out[n * 2] = '\0';
    return (out);
}

char    *md5_hex(const uint8_t *data, size_t len)
{
    (void)data;
    (void)len;
    uint8_t zero[16] = {0};
    return (bin_to_hex(zero, 16));
}

char    *sha256_hex(const uint8_t *data, size_t len)
{
    (void)data;
    (void)len;
    uint8_t zero[32] = {0};
    return (bin_to_hex(zero, 32));
}

const t_algo_api *algo_api(t_algo a)
{
    if (a == ALG_MD5)
        return (&g_md5);
    return (&g_sha256);
}

#include "ft_ssl.h"

static uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
static uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
static uint32_t Hh(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
static uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }
static uint32_t ROTL(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }

static const uint32_t T[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static const uint32_t S[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

static void md5_compress(t_md5_ctx *c, const uint8_t block[64])
{
	uint32_t a = c->a, b = c->b, cx = c->c, d = c->d, x[16];
	for (int i = 0; i < 16; i++)
		x[i] = (uint32_t)block[i * 4] | ((uint32_t)block[i * 4 + 1] << 8) | ((uint32_t)block[i * 4 + 2] << 16) | ((uint32_t)block[i * 4 + 3] << 24);
	for (int i = 0; i < 64; i++)
	{
		uint32_t f, g;
		if (i < 16)
		{
			f = F(b, cx, d);
			g = i;
		}
		else if (i < 32)
		{
			f = G(b, cx, d);
			g = (5 * i + 1) & 15;
		}
		else if (i < 48)
		{
			f = Hh(b, cx, d);
			g = (3 * i + 5) & 15;
		}
		else
		{
			f = I(b, cx, d);
			g = (7 * i) & 15;
		}
		uint32_t tmp = d;
		d = cx;
		cx = b;
		b = b + ROTL(a + f + T[i] + x[g], S[i]);
		a = tmp;
	}
	c->a += a;
	c->b += b;
	c->c += cx;
	c->d += d;
}

void md5_init(t_md5_ctx *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;
	ctx->total_len = 0;
	ctx->buf_len = 0;
}

void md5_update(t_md5_ctx *ctx, const uint8_t *data, size_t len)
{
	ctx->total_len += len;
	while (len)
	{
		size_t take = 64 - ctx->buf_len;
		if (take > len)
			take = len;
		ft_memcpy(ctx->buf + ctx->buf_len, data, take);
		ctx->buf_len += take;
		data += take;
		len -= take;
		if (ctx->buf_len == 64)
		{
			md5_compress(ctx, ctx->buf);
			ctx->buf_len = 0;
		}
	}
}

void md5_final(t_md5_ctx *ctx, uint8_t out[16])
{
	uint64_t bits = ctx->total_len * 8;
	uint8_t pad = 0x80;

	md5_update(ctx, &pad, 1);

	uint8_t zeroes[64] = {0};
	while (ctx->buf_len != 56)
		md5_update(ctx, zeroes, (ctx->buf_len < 56) ? (56 - ctx->buf_len) : (64 - ctx->buf_len));

	uint8_t lenle[8];
	for (int i = 0; i < 8; i++)
		lenle[i] = (uint8_t)((bits >> (8 * i)) & 0xff);

	md5_update(ctx, lenle, 8);

	uint32_t s[4] = {ctx->a, ctx->b, ctx->c, ctx->d};
	for (int i = 0; i < 4; i++)
	{
		out[i * 4 + 0] = (uint8_t)(s[i] & 0xff);
		out[i * 4 + 1] = (uint8_t)((s[i] >> 8) & 0xff);
		out[i * 4 + 2] = (uint8_t)((s[i] >> 16) & 0xff);
		out[i * 4 + 3] = (uint8_t)((s[i] >> 24) & 0xff);
	}
}

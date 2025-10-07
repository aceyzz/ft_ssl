#include "ft_ssl.h"

static uint32_t ROTR(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
static uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
static uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
static uint32_t Sig0(uint32_t x) { return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22); }
static uint32_t Sig1(uint32_t x) { return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25); }
static uint32_t sig0(uint32_t x) { return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3); }
static uint32_t sig1(uint32_t x) { return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10); }

static const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static void sha256_compress(t_sha256_ctx *c, const uint8_t block[64])
{
	uint32_t w[64];
	for (int i = 0; i < 16; i++)
		w[i] = ((uint32_t)block[i * 4] << 24) | ((uint32_t)block[i * 4 + 1] << 16) | ((uint32_t)block[i * 4 + 2] << 8) | ((uint32_t)block[i * 4 + 3]);

	for (int i = 16; i < 64; i++)
		w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];

	uint32_t a = c->h[0], b = c->h[1], d = c->h[3], e = c->h[4], f = c->h[5], g = c->h[6], h = c->h[7], cc = c->h[2];

	for (int i = 0; i < 64; i++)
	{
		uint32_t t1 = h + Sig1(e) + Ch(e, f, g) + K[i] + w[i];
		uint32_t t2 = Sig0(a) + Maj(a, b, cc);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = cc;
		cc = b;
		b = a;
		a = t1 + t2;
	}
	c->h[0] += a;
	c->h[1] += b;
	c->h[2] += cc;
	c->h[3] += d;
	c->h[4] += e;
	c->h[5] += f;
	c->h[6] += g;
	c->h[7] += h;
}

void sha256_init(t_sha256_ctx *ctx)
{
	ctx->h[0] = 0x6a09e667;
	ctx->h[1] = 0xbb67ae85;
	ctx->h[2] = 0x3c6ef372;
	ctx->h[3] = 0xa54ff53a;
	ctx->h[4] = 0x510e527f;
	ctx->h[5] = 0x9b05688c;
	ctx->h[6] = 0x1f83d9ab;
	ctx->h[7] = 0x5be0cd19;
	ctx->total_len = 0;
	ctx->buf_len = 0;
}

void sha256_update(t_sha256_ctx *ctx, const uint8_t *data, size_t len)
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
			sha256_compress(ctx, ctx->buf);
			ctx->buf_len = 0;
		}
	}
}

void sha256_final(t_sha256_ctx *ctx, uint8_t out[32])
{
	uint64_t bits = ctx->total_len * 8;
	uint8_t one = 0x80, zeroes[64] = {0};
	sha256_update(ctx, &one, 1);
	while (ctx->buf_len != 56)
		sha256_update(ctx, zeroes, (ctx->buf_len < 56) ? (56 - ctx->buf_len) : (64 - ctx->buf_len));
	uint8_t lenbe[8];
	for (int i = 0; i < 8; i++)
		lenbe[7 - i] = (uint8_t)((bits >> (8 * i)) & 0xff);
	sha256_update(ctx, lenbe, 8);
	for (int i = 0; i < 8; i++)
	{
		out[i * 4 + 0] = (uint8_t)((ctx->h[i] >> 24) & 0xff);
		out[i * 4 + 1] = (uint8_t)((ctx->h[i] >> 16) & 0xff);
		out[i * 4 + 2] = (uint8_t)((ctx->h[i] >> 8) & 0xff);
		out[i * 4 + 3] = (uint8_t)(ctx->h[i] & 0xff);
	}
}

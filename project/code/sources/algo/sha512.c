#include "ft_ssl.h"
#include "sha512.h"

// merci encore https://en.wikipedia.org/wiki/SHA-2#Pseudocode

// operations
static uint64_t ROTR(uint64_t x, uint64_t n) { return (x >> n) | (x << (64 - n)); }
static uint64_t Ch(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (~x & z); }
static uint64_t Maj(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (x & z) ^ (y & z); }
static uint64_t Sig0(uint64_t x) { return ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39); }
static uint64_t Sig1(uint64_t x) { return ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41); }
static uint64_t sig0(uint64_t x) { return ROTR(x, 1) ^ ROTR(x, 8) ^ (x >> 7); }
static uint64_t sig1(uint64_t x) { return ROTR(x, 19) ^ ROTR(x, 61) ^ (x >> 6); }

// constantes K[80] pour les rounds SHA-512
static const uint64_t K[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

// compresse bloc de 128 octets
static void sha512_compress(t_sha512_ctx *c, const uint8_t block[128])
{
	uint64_t w[80];
	for (int i = 0; i < 16; i++) {
		int j = i * 8;
		w[i] = ((uint64_t)block[j + 0] << 56) | ((uint64_t)block[j + 1] << 48) |
		       ((uint64_t)block[j + 2] << 40) | ((uint64_t)block[j + 3] << 32) |
		       ((uint64_t)block[j + 4] << 24) | ((uint64_t)block[j + 5] << 16) |
		       ((uint64_t)block[j + 6] <<  8) | ((uint64_t)block[j + 7] <<  0);
	}
	for (int i = 16; i < 80; i++)
		w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];

	uint64_t a = c->h[0], b = c->h[1], cc = c->h[2], d = c->h[3];
	uint64_t e = c->h[4], f = c->h[5], g = c->h[6], h = c->h[7];

	for (int i = 0; i < 80; i++) {
		uint64_t t1 = h + Sig1(e) + Ch(e, f, g) + K[i] + w[i];
		uint64_t t2 = Sig0(a) + Maj(a, b, cc);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = cc;
		cc = b;
		b = a;
		a = t1 + t2;
	}
	c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d;
	c->h[4] += e; c->h[5] += f; c->h[6] += g; c->h[7] += h;
}

void sha512_init(t_sha512_ctx *ctx)
{
	ctx->h[0] = 0x6a09e667f3bcc908ULL;
	ctx->h[1] = 0xbb67ae8584caa73bULL;
	ctx->h[2] = 0x3c6ef372fe94f82bULL;
	ctx->h[3] = 0xa54ff53a5f1d36f1ULL;
	ctx->h[4] = 0x510e527fade682d1ULL;
	ctx->h[5] = 0x9b05688c2b3e6c1fULL;
	ctx->h[6] = 0x1f83d9abfb41bd6bULL;
	ctx->h[7] = 0x5be0cd19137e2179ULL;
	ctx->total_low = 0;
	ctx->total_high = 0;
	ctx->buf_len = 0;
}

static void add_total_bits(t_sha512_ctx *ctx, size_t add_bytes)
{
	// total += add_bytes * 8 sur 128 bits (high:low)
	uint64_t add_low = (uint64_t)add_bytes << 3;
	uint64_t prev_low = ctx->total_low;
	ctx->total_low += add_low;
	ctx->total_high += (ctx->total_low < prev_low) ? 1 : 0;
}

void sha512_update(t_sha512_ctx *ctx, const uint8_t *data, size_t len)
{
	if (len) add_total_bits(ctx, len);
	while (len) {
		size_t take = 128 - ctx->buf_len;
		if (take > len) take = len;
		ft_memcpy(ctx->buf + ctx->buf_len, data, take);
		ctx->buf_len += take;
		data += take;
		len -= take;
		if (ctx->buf_len == 128) {
			sha512_compress(ctx, ctx->buf);
			ctx->buf_len = 0;
		}
	}
}

void sha512_final(t_sha512_ctx *ctx, uint8_t out[64])
{
	// snapshot de la longueur en bits AVANT padding
	uint64_t len_high = ctx->total_high;
	uint64_t len_low  = ctx->total_low;

	uint8_t one = 0x80, zeroes[128] = {0};

	// padding: ajoute 0x80 puis des 0 jusqu'à buf_len == 112
	sha512_update(ctx, &one, 1);
	while (ctx->buf_len != 112)
		sha512_update(ctx, zeroes, (ctx->buf_len < 112) ? (112 - ctx->buf_len) : (128 - ctx->buf_len));

	// longueur (128 bits big-endian) basée sur le snapshot
	uint8_t lenbe[16];
	for (int i = 0; i < 8; i++) {
		lenbe[7 - i]  = (uint8_t)((len_high >> (8 * i)) & 0xff);
		lenbe[15 - i] = (uint8_t)((len_low  >> (8 * i)) & 0xff);
	}
	sha512_update(ctx, lenbe, 16);

	// sortie big-endian
	for (int i = 0; i < 8; i++) {
		out[i*8+0] = (uint8_t)((ctx->h[i] >> 56) & 0xff);
		out[i*8+1] = (uint8_t)((ctx->h[i] >> 48) & 0xff);
		out[i*8+2] = (uint8_t)((ctx->h[i] >> 40) & 0xff);
		out[i*8+3] = (uint8_t)((ctx->h[i] >> 32) & 0xff);
		out[i*8+4] = (uint8_t)((ctx->h[i] >> 24) & 0xff);
		out[i*8+5] = (uint8_t)((ctx->h[i] >> 16) & 0xff);
		out[i*8+6] = (uint8_t)((ctx->h[i] >>  8) & 0xff);
		out[i*8+7] = (uint8_t)((ctx->h[i] >>  0) & 0xff);
	}
}

char *sha512_hex(const uint8_t *data, size_t len)
{
	t_sha512_ctx ctx; uint8_t digest[64];
	sha512_init(&ctx);
	if (data && len) sha512_update(&ctx, data, len);
	sha512_final(&ctx, digest);
	return (bin_to_hex(digest, 64));
}

char *sha512_fd_hex(int fd)
{
	t_sha512_ctx ctx; uint8_t digest[64];
	uint8_t buf[4096]; ssize_t n;
	sha512_init(&ctx);
	while ((n = read(fd, buf, sizeof(buf))) > 0)
		sha512_update(&ctx, buf, (size_t)n);
	if (n < 0) return NULL;
	sha512_final(&ctx, digest);
	return (bin_to_hex(digest, 64));
}

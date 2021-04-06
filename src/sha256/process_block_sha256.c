#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_sha256.h"

void				process_block_sha256(uint32_t *block, uint32_t *state)
{
	uint32_t		a = state[0];
	uint32_t		b = state[1];
	uint32_t		c = state[2];
	uint32_t		d = state[3];
	uint32_t		e = state[4];
	uint32_t		f = state[5];
	uint32_t		g = state[6];
	uint32_t		h = state[7];

	uint32_t		msa[64]; // message schedule array
	uint32_t		temp1;
	uint32_t		temp2;
	const uint32_t	*crt = g_sha256_cube_root_table;

	for (uint32_t i = 0; i < 16; i++)
		msa[i] = block[i];
	for (uint32_t t = 16; t < 64; t++)
		msa[t] = (SSIG1(msa[t-2])) + msa[t-7] + (SSIG0(msa[t-15])) + msa[t-16];

	for (uint32_t i = 0; i < 64; i++)
	{
		temp1 = h + (BSIG1(e)) + (CH(e, f, g)) + crt[i] + msa[i];
		temp2 = (BSIG0(a)) + (MAJ(a, b, c));

		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

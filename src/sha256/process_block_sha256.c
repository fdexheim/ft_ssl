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

	for (uint32_t iter = 0; iter < 16; iter++)
	{
		msa[iter] = block[iter];
	}

	for (uint32_t iter = 16; iter < 64; iter++)
	{
		uint32_t		temp1;
		uint32_t		temp2;

		temp1 = (ROTR32(msa[iter - 15], 7) ^ ROTR32(msa[iter - 15], 18) ^ SHR(msa[iter - 15], 3));
		temp2 = (ROTR32(msa[iter - 2], 17) ^ ROTR32(msa[iter - 2], 19) ^ SHR(msa[iter - 2], 10));
		msa[iter] = msa[iter - 16] + temp1 + msa[iter - 7] + temp2;
	}

	for (uint32_t iter = 0; iter < 64; iter++)
	{
		uint32_t		ch;
		uint32_t		maj;
		uint32_t		s1;
		uint32_t		s0;
		uint32_t		temp1;
		uint32_t		temp2;

		s1 = ROTR32(e, 6) ^ ROTR32(e, 11) ^ ROTR32(e, 25);
		ch = (e & f) ^ ((~e) & g);
		temp1 = h + s1 + ch + g_sha256_cube_root_table[iter] + msa[iter];

		s0 = ROTR32(a, 2) ^ ROTR32(a, 13) ^ ROTR32(a, 22);
		maj = (a & b) ^ (a & c) ^ (b & c);
		temp2 = s0 + maj;

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

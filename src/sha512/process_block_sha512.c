#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_sha512.h"

void				process_block_sha512(uint64_t *block, uint64_t *state)
{
	uint64_t		a = state[0];
	uint64_t		b = state[1];
	uint64_t		c = state[2];
	uint64_t		d = state[3];
	uint64_t		e = state[4];
	uint64_t		f = state[5];
	uint64_t		g = state[6];
	uint64_t		h = state[7];
	uint64_t		msa[80]; // message schedule array

	for (uint64_t iter = 0; iter < 16; iter++)
	{
		msa[iter] = block[iter];
	}

	for (uint64_t iter = 16; iter < 80; iter++)
	{
		uint64_t		s1;
		uint64_t		s0;

		s0 = (ROTR64(msa[iter - 15], 1) ^ ROTR64(msa[iter - 15], 8) ^ SHR(msa[iter - 15], 7));
		s1 = (ROTR64(msa[iter - 2], 19) ^ ROTR64(msa[iter - 2], 61) ^ SHR(msa[iter - 2], 6));
		msa[iter] = msa[iter - 16] + s0 + msa[iter - 7] + s1;
	}

	for (uint64_t iter = 0; iter < 80; iter++)
	{
		uint64_t		ch;
		uint64_t		maj;
		uint64_t		s1;
		uint64_t		s0;
		uint64_t		temp1;
		uint64_t		temp2;

		s1 = ROTR64(e, 14) ^ ROTR64(e, 18) ^ ROTR64(e, 41);
		ch = (e & f) ^ ((~e) & g);
		temp1 = h + s1 + ch + g_sha512_constant_table[iter] + msa[iter];

		s0 = ROTR64(a, 28) ^ ROTR64(a, 34) ^ ROTR64(a, 39);
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

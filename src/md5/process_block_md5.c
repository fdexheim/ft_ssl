#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_md5.h"

static void				iter_step(uint32_t *block, uint32_t *state_copy,
	uint32_t iter)
{
	const uint32_t	*sht = g_md5_shift_table;
	const uint32_t	*sint = g_md5_sin_table;
	const uint32_t	*bit = g_md5_block_index_table;
	uint32_t		*sc = state_copy;
	uint32_t		si; // "state index", used to make the juggling between the 4 state values below less unbearable"

	si = iter % 4 == 0 ? 0 : 4 - iter % 4;

	if (iter < 16)
	{
		sc[si] += F(sc[(si + 1) % 4], sc[(si + 2) % 4], sc[(si + 3) % 4])
			+ block[bit[iter]] + sint[iter];
		sc[si] = ROTATE_LEFT(sc[si], sht[(iter % 4)]);
	}
	else if (iter < 32)
	{
		sc[si] += G(sc[(si + 1) % 4], sc[(si + 2) % 4], sc[(si + 3) % 4])
			+ block[bit[iter]] + sint[iter];
		sc[si] = ROTATE_LEFT(sc[si], sht[(iter % 4) + 4]);
	}
	else if (iter < 48)
	{
		sc[si] += H(sc[(si + 1) % 4], sc[(si + 2) % 4], sc[(si + 3) % 4])
			+ block[bit[iter]] + sint[iter];
		sc[si] = ROTATE_LEFT(sc[si], sht[(iter % 4) + 8]);
	}
	else if (iter < 64)
	{
		sc[si] += I(sc[(si + 1) % 4], sc[(si + 2) % 4], sc[(si + 3) % 4])
			+ block[bit[iter]] + sint[iter];
		sc[si] = ROTATE_LEFT(sc[si], sht[(iter % 4) + 12]);
	}

	sc[si] += sc[(si + 1) % 4];
}

//------------------------------------------------------------------------------
void				process_block_md5(uint32_t *block, uint32_t *state)
{
	uint32_t		state_copy[4] = { state[0], state[1], state[2], state[3] };

	for (uint32_t i = 0; i < 64; i++)
	{
		iter_step(block, state_copy, i);
	}
	state[0] += state_copy[0];
	state[1] += state_copy[1];
	state[2] += state_copy[2];
	state[3] += state_copy[3];
}

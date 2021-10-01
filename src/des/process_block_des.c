#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
void			custom_bit_print(uint8_t *key, uint8_t key_size, uint8_t bit_size)
{
	for (uint8_t i = 0; i < key_size; i++)
	{
		for (uint8_t j = bit_size; j < 8; j++)
			ft_putstr(" ");
		for (int8_t k = bit_size - 1; k >= 0; k--)
		{
			if (ft_testbit(key[i], k) == true)
				write(1, "1", 1);
			else
				write(1, "0", 1);
		}
		ft_putstr("  ");
	}
	ft_putstr("\n");
}

//------------------------------------------------------------------------------
void			custom_bit_lshift(uint8_t *key, uint8_t key_size, uint8_t bit_number_wrap_point)
{
	bool		big_wrap = false;

	for (uint8_t i = 0; i < key_size; i++)
	{
		key[i] = key[i] << 1;
		if (ft_testbit(key[i], bit_number_wrap_point) == true)
		{
			if (i == 0)
				big_wrap = true;
			else
				ft_set_bit_8(&key[i - 1], 0);
			ft_unset_bit_8(&key[i], bit_number_wrap_point);
		}
	}
	if (big_wrap)
		ft_set_bit_8(&key[key_size - 1], 0);
}

//------------------------------------------------------------------------------
void			permute(uint8_t *old_key, uint8_t *new_key, uint8_t old_cell_size,
		uint8_t new_cell_size, const uint8_t *permutation_table, uint8_t permutations)
{
	uint8_t old_cell_number = 0;
	uint8_t old_array_index = 0;
	uint8_t new_cell_number = 0;
	uint8_t new_array_index = 0;

	for (uint8_t i = 0; i < permutations; i++)
	{
		old_array_index = (permutation_table[i] - 1) / old_cell_size;
		old_cell_number = (permutation_table[i] - 1) % old_cell_size;
		old_cell_number = (old_cell_size - 1) - old_cell_number;
		if (ft_testbit(old_key[old_array_index], old_cell_number) == true)
		{
			new_array_index = i / new_cell_size;
			new_cell_number = i % new_cell_size;
			new_cell_number = (new_cell_size - 1) - new_cell_number;
			ft_set_bit_8(&new_key[new_array_index], new_cell_number);
		}
	}
}

//------------------------------------------------------------------------------
void			cd_tables(uint8_t *kplus, uint8_t **cd)
{
	for (uint8_t j = 0; j < 8; j++)
	{
		cd[0][j] = kplus[j];
	}
	for (uint8_t i = 1; i < 17; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			cd[i][j] = cd[i - 1][j];
		}
		for (uint8_t j = 0; j < g_shift_table[i - 1]; j++)
		{
			custom_bit_lshift(cd[i], 4, 7);
			custom_bit_lshift(&cd[i][4], 4, 7);
		}
	}
}

//------------------------------------------------------------------------------
void			free_subkeys(t_des_subkeys *sk)
{
	for (int i = 0; i < 17; i++)
	{
		free(sk->cd[i]);
		free(sk->k[i]);
	}
	free(sk->cd);
	free(sk->k);
	free(sk);
}

//------------------------------------------------------------------------------
// STEP 1 : CALCULATE SUBKEYS
//------------------------------------------------------------------------------
t_des_subkeys			*get_subkeys(uint8_t * key)
{
	t_des_subkeys		*ret;

	if ((ret = malloc(sizeof(t_des_subkeys))) == NULL)
		return (NULL);
	ret->cd = malloc(sizeof(uint8_t *) * 17);
	ret->k = malloc(sizeof(uint8_t *) * 17);
	if (!ret->cd || !ret->k)
	{
		ft_putstr("[Error] Bad malloc in des subkey allocation\n");
		return (NULL);
	}
	for (int i = 0; i < 17; i++)
	{
		ret->cd[i] = malloc(sizeof(uint8_t) * 8);
		ret->k[i] = malloc(sizeof(uint8_t) * 8);
		if (!ret->cd[i] || !ret->k[i])
		{
			ft_putstr("[Error] Bad malloc in des subkey allocation\n");
			return (NULL);
		}
		ft_bzero(ret->cd[i], sizeof(uint8_t) * 8);
		ft_bzero(ret->k[i], sizeof(uint8_t) * 8);
	}
	ft_bzero(ret->kplus, sizeof(uint8_t) * 8);
	permute(key, ret->kplus, 8, 7, g_pc1_table, 56);
	cd_tables(ret->kplus, ret->cd);
	for (int i = 0; i < 17; i++)
		permute(ret->cd[i], ret->k[i], 7, 6, g_pc2_table, 48);

	return (ret);
}

//------------------------------------------------------------------------------
// STEP 2 : ENCODE 64 BIT BLOCKS
//------------------------------------------------------------------------------
void			process_block_des(uint8_t *block, t_des_subkeys *subkeys)
{
	uint8_t		**l;
	uint8_t		**r;
	uint8_t		ip[8];

	if ((l = malloc(sizeof(uint8_t *) * 17)) == NULL
		|| (r = malloc(sizeof(uint8_t *) * 17)) == NULL)
	{
		ft_putstr("[Error] Bad malloc in process_block_des\n");
		return ;
	}
	for (int i = 0; i < 17; i++)
	{
		l[i] = malloc(sizeof(uint8_t) * 8);
		r[i] = malloc(sizeof(uint8_t) * 8);
		if (!l[i] || !r[i])
		{
			ft_putstr("[Error] Bad malloc in process_block_des\n");
			return ;
		}
	}
	// get ip with ip table permutation
	ft_bzero(ip, sizeof(uint8_t) * 8);
	permute(block, ip, 8, 8, g_ip_table, 64);

	// splitting ip into L0 and R0
	for (int i = 0; i < 4; i++)
	{
		l[0][i * 2] = ft_extract_bits(ip[i], 4, 7);
		l[0][i * 2 + 1] = ft_extract_bits(ip[i], 0, 3);
		r[0][i * 2] = ft_extract_bits(ip[i + 4], 4, 7);
		r[0][i * 2 + 1] = ft_extract_bits(ip[i + 4], 0, 3);
	}

	for (int i = 1; i < 17; i++)
	{
		uint8_t e[8];
		uint8_t ek_xor[8];
		uint8_t s[8];
		uint8_t p[8];

		ft_bzero(e, sizeof(uint8_t) * 8);
		ft_bzero(ek_xor, sizeof(uint8_t) * 8);
		ft_bzero(s, sizeof(uint8_t) * 8);
		ft_bzero(p, sizeof(uint8_t) * 8);

		// We get L in this iteration from R in previous iteration
		for (int j = 0; j < 8; j++)
			l[i][j] = r[i - 1][j];
		permute(r[i - 1], e, 4, 6, g_ebit_selection_table, 48);

		// "S boxes" step
		for (int j = 0; j < 8; j++)
		{
			const uint8_t *tables_s[8] = { g_s1_table, g_s2_table, g_s3_table,
				g_s4_table, g_s5_table, g_s6_table, g_s7_table, g_s8_table };
			uint8_t row_s = 0; // from first and last bits of ek_xor
			uint8_t col_s = 0; // from the 4 bits in between

			ek_xor[j] = subkeys->k[i][j] ^ e[j];
			if (ft_testbit(ek_xor[j], 0))
				row_s += 1;
			if (ft_testbit(ek_xor[j], 5))
				row_s += 2;
			col_s = ft_extract_bits(ek_xor[j], 1, 4);
			s[j] = tables_s[j][16 * row_s + col_s];
		}

		// P table permutation
		permute(s, p, 4, 4, g_p_table, 32);

		// We finally can calculate R for this iteration
		for (int j = 0; j < 8; j++)
			r[i][j] = l[i - 1][j] ^ p[j];
	}

	uint8_t rl_16[8];
	uint8_t ip_1[8];
	ft_bzero(rl_16, sizeof(uint8_t) * 8);
	ft_bzero(ip_1, sizeof(uint8_t) * 8);
	// merge l[16] and r[16]
	for (int i = 0; i < 4; i++)
	{
		rl_16[i] = (r[16][i * 2] << 4) + r[16][i * 2 + 1];
		rl_16[i + 4] = (l[16][i * 2] << 4) + l[16][i * 2 + 1];
	}
	permute(rl_16, ip_1, 8, 8, g_ip1_table, 64);
	ft_putstr("RL16 : ");
	custom_bit_print(rl_16, 8, 8);
	// We finally have our translated block
	ft_putstr("IP_1 : ");
	custom_bit_print(ip_1, 8, 8);
	
	for (int i = 0; i < 17; i++)
	{
		free(l[i]);
		free(r[i]);
	}
	free(l);
	free(r);
}

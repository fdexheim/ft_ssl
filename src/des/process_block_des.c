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
uint8_t			***calc_subkeys(uint8_t *key)
{
	(void)key;
	return (NULL);
}

//------------------------------------------------------------------------------
void			process_block_des(uint8_t *block, uint8_t *key)
{
	ft_putstr(">>process_blockey_des called\n");
	uint8_t		tmp_block[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	block = tmp_block;

	uint8_t		kplus[8];
	uint8_t		**cd;
	uint8_t		**k;
	uint8_t		**l;
	uint8_t		**r;
	uint8_t		ip[8];


	ft_bzero(kplus, sizeof(uint8_t) * 8);
	ft_bzero(ip, sizeof(uint8_t) * 8);

	// allocation subkeys
	cd = malloc(sizeof(uint8_t *) * 17);
	k = malloc(sizeof(uint8_t *) * 17);
	l = malloc(sizeof(uint8_t *) * 17);
	r = malloc(sizeof(uint8_t *) * 17);
	if (!cd || !k || !l || !r)
	{
		ft_putstr("[Error] Bad malloc in des blockey processing\n");
		return;
	}
	for (int i = 0; i < 17; i++)
	{
		cd[i] = malloc(sizeof(uint8_t) * 8);
		k[i] = malloc(sizeof(uint8_t) * 8);
		l[i] = malloc(sizeof(uint8_t) * 8);
		r[i] = malloc(sizeof(uint8_t) * 8);
		if (!cd[i] || !k[i] || !l[i] || !r[i])
		{
			ft_putstr("[Error] Bad malloc in des blockey processing\n");
			return;
		}
		ft_bzero(cd[i], sizeof(uint8_t) * 8);
		ft_bzero(k[i], sizeof(uint8_t) * 8);
		ft_bzero(l[i], sizeof(uint8_t) * 8);
		ft_bzero(r[i], sizeof(uint8_t) * 8);
	}

	//--------------------------------------------------//
	// STEP 1 : CALCULATE SUBKEYS						//
	//--------------------------------------------------//
	permute(key, kplus, 8, 7, g_pc1_table, 56);
	cd_tables(kplus, cd);
	for (int i = 0; i < 17; i++)
		permute(cd[i], k[i], 7, 6, g_pc2_table, 48);
	//--------------------------------------------------//
	// STEP 2 : ENCODE 64 BIT BLOCKS					//
	//--------------------------------------------------//
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

		for (int j = 0; j < 8; j++)
			l[i][j] = r[i - 1][j];
		ft_bzero(e, sizeof(uint8_t) * 8);
		ft_bzero(ek_xor, sizeof(uint8_t) * 8);
		permute(r[i - 1], e, 4, 6, g_ebit_selection_table, 48);
		for (int j = 0; j < 8; j++)
		{
			ek_xor[j] = k[i][j] ^ e[j];
		}


		ft_putstr("E  ");
		if (i < 10)
			ft_putstr(" ");
		ft_put_size_t(i);
		ft_putstr("    : ");
		custom_bit_print(e, 8, 6);
		ft_putstr("EK ");
		if (i < 10)
			ft_putstr(" ");
		ft_put_size_t(i);
		ft_putstr("    : ");
		custom_bit_print(ek_xor, 8, 6);


	}
	ft_putstr("\n");


	//--------------------------------------------------//
	// DEBUG DUMPS										//
	//--------------------------------------------------//
/*
	// dump subkeys K/K+
	ft_putstr("K        : ");
	custom_bit_print(key, 8, 8);
	ft_putstr("K+       : ");
	custom_bit_print(kplus, 8, 7);
	ft_putstr("\n");

	// dump subkeys C/D
	for (int i = 0; i < 17; i++)
	{
		ft_putstr("CD");
		ft_put_size_t(i);
		if (i < 10)
			ft_putstr(" ");
		ft_putstr("     : ");
		custom_bit_print(cd[i], 8, 7);
	}
	ft_putstr("\n");

	// dump subkeys K
	for (int i = 0; i < 17; i++)
	{
		ft_putstr("K");
		ft_put_size_t(i);
		if (i < 10)
			ft_putstr(" ");
		ft_putstr("      : ");
		custom_bit_print(k[i], 8, 6);
	}
	ft_putstr("\n");
*/
	// dump LR subkeys
	ft_putstr("Block    : ");
	custom_bit_print(block, 8, 8);
	ft_putstr("IP       : ");
	custom_bit_print(ip, 8, 8);
	for (int i = 0; i < 17; i++)
	{
		ft_putstr("L");
		ft_put_size_t(i);
		if (i < 10)
			ft_putstr(" ");
		ft_putstr("      : ");
		custom_bit_print(l[i], 8, 4);
		ft_putstr("R");
		ft_put_size_t(i);
		if (i < 10)
			ft_putstr(" ");
		ft_putstr("      : ");
		custom_bit_print(r[i], 8, 4);

	}
	ft_putstr("\n");




	// free subkeys
	for (int i = 0; i < 17; i++)
	{
		free(cd[i]);
		free(k[i]);
	}
	free(cd);
	free(k);

	ft_putstr(">>process_blockey_des ended\n");
}

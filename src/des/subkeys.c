#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_des.h"

//------------------------------------------------------------------------------
// STEP 1 : CALCULATE SUBKEYS
//------------------------------------------------------------------------------
void				free_subkeys(t_des_subkeys *sk)
{
	if (sk == NULL)
		return ;
	if (sk->cd != NULL)
		free_array((void*)sk->cd);
	if (sk->cd != NULL)
		free_array((void*)sk->k);
	free(sk);
}

//------------------------------------------------------------------------------
void				calculate_subkeys(t_des_subkeys *sk, uint8_t *key)
{
	permute(key, sk->kplus, 8, 7, g_pc1_table, 56);
	for (uint8_t j = 0; j < 8; j++)
	{
		sk->cd[0][j] = sk->kplus[j];
	}
	for (uint8_t i = 1; i < 17; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			sk->cd[i][j] = sk->cd[i - 1][j];
		}
		for (uint8_t j = 0; j < g_shift_table[i - 1]; j++)
		{
			custom_bit_lshift(sk->cd[i], 4, 7);
			custom_bit_lshift(&sk->cd[i][4], 4, 7);
		}
	}
	for (int i = 0; i < 17; i++)
		permute(sk->cd[i], sk->k[i], 7, 6, g_pc2_table, 48);
}

//------------------------------------------------------------------------------
t_des_subkeys		*allocate_subkeys()
{
	t_des_subkeys	*ret;

	if ((ret = malloc(sizeof(t_des_subkeys))) == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(t_des_subkeys));
	ret->cd = (uint8_t**)bootleg_calloc(17, sizeof(uint8_t) * 8);
	ret->k = (uint8_t**)bootleg_calloc(17, sizeof(uint8_t) * 8);
	ft_bzero(ret->kplus, sizeof(uint8_t) * 8);
	return (ret);
}

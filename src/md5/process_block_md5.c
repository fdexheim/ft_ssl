#include "../../inc/ft_ssl.h"
#include "../../inc/ft_ssl_md5.h"

//------------------------------------------------------------------------------
void				process_block_md5(uint32_t *block, uint32_t *state)
{
	uint32_t		a = state[0];
	uint32_t		b = state[1];
	uint32_t		c = state[2];
	uint32_t		d = state[3];
	const uint32_t	*sht = g_md5_shift_table;
	const uint32_t	*sit = g_md5_sin_table;

	FF(a, b, c, d, block[0], sht[0], sit[0]);
	FF(d, a, b, c, block[1], sht[1], sit[1]);
	FF(c, d, a, b, block[2], sht[2], sit[2]);
	FF(b, c, d, a, block[3], sht[3], sit[3]);
	FF(a, b, c, d, block[4], sht[0], sit[4]);
	FF(d, a, b, c, block[5], sht[1], sit[5]);
	FF(c, d, a, b, block[6], sht[2], sit[6]);
	FF(b, c, d, a, block[7], sht[3], sit[7]);
	FF(a, b, c, d, block[8], sht[0], sit[8]);
	FF(d, a, b, c, block[9], sht[1], sit[9]);
	FF(c, d, a, b, block[10], sht[2], sit[10]);
	FF(b, c, d, a, block[11], sht[3], sit[11]);
	FF(a, b, c, d, block[12], sht[0], sit[12]);
	FF(d, a, b, c, block[13], sht[1], sit[13]);
	FF(c, d, a, b, block[14], sht[2], sit[14]);
	FF(b, c, d, a, block[15], sht[3], sit[15]);

	GG(a, b, c, d, block[1], sht[4], sit[16]);
	GG(d, a, b, c, block[6], sht[5], sit[17]);
	GG(c, d, a, b, block[11], sht[6], sit[18]);
	GG(b, c, d, a, block[0], sht[7], sit[19]);
	GG(a, b, c, d, block[5], sht[4], sit[20]);
	GG(d, a, b, c, block[10], sht[5], sit[21]);
	GG(c, d, a, b, block[15], sht[6], sit[22]);
	GG(b, c, d, a, block[4], sht[7], sit[23]);
	GG(a, b, c, d, block[9], sht[4], sit[24]);
	GG(d, a, b, c, block[14], sht[5], sit[25]);
	GG(c, d, a, b, block[3], sht[6], sit[26]);
	GG(b, c, d, a, block[8], sht[7], sit[27]);
	GG(a, b, c, d, block[13], sht[4], sit[28]);
	GG(d, a, b, c, block[2], sht[5], sit[29]);
	GG(c, d, a, b, block[7], sht[6], sit[30]);
	GG(b, c, d, a, block[12], sht[7], sit[31]);

	HH(a, b, c, d, block[5], sht[8], sit[32]);
	HH(d, a, b, c, block[8], sht[9], sit[33]);
	HH(c, d, a, b, block[11], sht[10], sit[34]);
	HH(b, c, d, a, block[14], sht[11], sit[35]);
	HH(a, b, c, d, block[1], sht[8], sit[36]);
	HH(d, a, b, c, block[4], sht[9], sit[37]);
	HH(c, d, a, b, block[7], sht[10], sit[38]);
	HH(b, c, d, a, block[10], sht[11], sit[39]);
	HH(a, b, c, d, block[13], sht[8], sit[40]);
	HH(d, a, b, c, block[0], sht[9], sit[41]);
	HH(c, d, a, b, block[3], sht[10], sit[42]);
	HH(b, c, d, a, block[6], sht[11], sit[43]);
	HH(a, b, c, d, block[9], sht[8], sit[44]);
	HH(d, a, b, c, block[12], sht[9], sit[45]);
	HH(c, d, a, b, block[15], sht[10], sit[46]);
	HH(b, c, d, a, block[2], sht[11], sit[47]);

	II(a, b, c, d, block[0], sht[12], sit[48]);
	II(d, a, b, c, block[7], sht[13], sit[49]);
	II(c, d, a, b, block[14], sht[14], sit[50]);
	II(b, c, d, a, block[5], sht[15], sit[51]);
	II(a, b, c, d, block[12], sht[12], sit[52]);
	II(d, a, b, c, block[3], sht[13], sit[53]);
	II(c, d, a, b, block[10], sht[14], sit[54]);
	II(b, c, d, a, block[1], sht[15], sit[55]);
	II(a, b, c, d, block[8], sht[12], sit[56]);
	II(d, a, b, c, block[15], sht[13], sit[57]);
	II(c, d, a, b, block[6], sht[14], sit[58]);
	II(b, c, d, a, block[13], sht[15], sit[59]);
	II(a, b, c, d, block[4], sht[12], sit[60]);
	II(d, a, b, c, block[11], sht[13], sit[61]);
	II(c, d, a, b, block[2], sht[14], sit[62]);
	II(b, c, d, a, block[9], sht[15], sit[63]);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

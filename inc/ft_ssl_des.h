#ifndef FT_SSL_DES_H
# define FT_SSL_DES_H

static const uint8_t		g_pc1_table[56] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

static const uint8_t		g_pc2_table[48] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static const uint8_t		g_shift_table[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static const uint8_t		g_ip_table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};


static const uint8_t	g_tmp_keys[3][8] = {
	{	0x13, 0x34, 0x57, 0x79,
		0x9b, 0xbc, 0xdf, 0xf1 },
	{	0x01, 0x02, 0x03, 0x04,
		0x05, 0x06, 0x07, 0x08 },
	{	0x09, 0x0a, 0x0b, 0x0c,
		0x0d, 0x0e, 0x0f, 0x00
	}
}; // fixed one for now will implement key initialization later



void						display_des(t_ssl_env *env, char *src, void *state,
	bool string_mode);
void						parse_des(t_ssl_env *env, char **args);
void						process_block_des(uint8_t *block, uint8_t *key);

#endif

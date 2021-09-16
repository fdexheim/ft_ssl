#ifndef FT_SSL_DES_H
# define FT_SSL_DES_H

static uint8_t g_pc1_table[72] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};


void						display_des(t_ssl_env *env, char *src, void *state, bool string_mode);
void						parse_des(t_ssl_env *env, char **args);
void						process_block_des(uint8_t *block, uint8_t *key);

#endif

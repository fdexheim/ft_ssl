#ifndef FT_SSL_DES_H
# define FT_SSL_DES_H


void						display_des(t_ssl_env *env, char *src, void *state, bool string_mode);
void						parse_des(t_ssl_env *env, char **args);
void						process_block_des(uint32_t *block, uint32_t *state);

#endif

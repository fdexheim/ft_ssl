#ifndef FT_SSL_BASE64_H
# define FT_SSL_BASE64_H

static const char		g_base64_table[64] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};


#define SHL(word, bits) word << bits
#define SHR(word, bits) word >> bits


void						display_base64(t_ssl_data *output,
	char *file_output, bool decrypt);
void						parse_base64(t_ssl_env *env, char **args);
void						process_block_base64(char *input_block,
	char *output_block, bool decrypt, size_t pad_size);

#endif

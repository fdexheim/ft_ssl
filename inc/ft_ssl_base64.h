#ifndef FT_SSL_BASE64_H
# define FT_SSL_BASE64_H

static const char		g_base64_table[64] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};

#define SHL(word, bits) word << bits
#define SHR(word, bits) word >> bits

#endif

#ifndef _H2O_HUFFMAN_H
#define _H2O_HUFFMAN_H

#include <stddef.h>
#include <stdint.h>

void huffman_decode(uint8_t *sbuf, size_t sbuflen, uint8_t *dbuf,
		    size_t dbuflen);

#endif

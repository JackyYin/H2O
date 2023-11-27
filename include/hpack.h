#ifndef _H2O_HPACK_H
#define _H2O_HPACK_H

#include "hashmap.h"
#include "hdt.h"

void hpack_header_decode(hdt *dytbl, struct h2o_hashmap *headers,
			 uint8_t **rbuf);

int hpack_header_encode(hdt *dytbl, uint8_t **wbuf, char *name, size_t namelen,
			char *value, size_t valuelen);
#endif

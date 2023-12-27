#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hpack.h"
#include "huffman.h"

#define GET_ST_NAME(index) (static_tables[index - 1][0])
#define GET_ST_VALUE(index) (static_tables[index - 1][1])
#define ST_NUM_OF_ENTRIES (sizeof(static_tables) / sizeof(static_tables[0]))

static char *static_tables[][2] = {
    {":authority", ""},
    {":method", "GET"},
    {":method", "POST"},
    {":path", "/"},
    {":path", "/index.html"},
    {":scheme", "http"},
    {":scheme", "https"},
    {":status", "200"},
    {":status", "204"},
    {":status", "206"},
    {":status", "304"},
    {":status", "400"},
    {":status", "404"},
    {":status", "500"},
    {"accept-charset", ""},
    {"accept-encoding", "gzip, deflate"},
    {"accept-language", ""},
    {"accept-ranges", ""},
    {"accept", ""},
    {"accept-control-allow-origin", ""},
    {"age", ""},
    {"allow", ""},
    {"authorization", ""},
    {"cache-control", ""},
    {"content-disposition", ""},
    {"content-encoding", ""},
    {"content-language", ""},
    {"content-length", ""},
    {"content-location", ""},
    {"content-range", ""},
    {"content-type", ""},
    {"cookie", ""},
    {"data", ""},
    {"etag", ""},
    {"expect", ""},
    {"expires", ""},
    {"from", ""},
    {"host", ""},
    {"if-match", ""},
    {"if-modified-since", ""},
    {"if-none-match", ""},
    {"if-range", ""},
    {"if-unmodified-since", ""},
    {"last-modified", ""},
    {"link", ""},
    {"location", ""},
    {"max-forwards", ""},
    {"proxy-authenticate", ""},
    {"proxy-authorization", ""},
    {"range", ""},
    {"referer", ""},
    {"refresh", ""},
    {"retry-after", ""},
    {"server", ""},
    {"set-cookie", ""},
    {"strict-transport-security", ""},
    {"transfer-encoding", ""},
    {"user-agent", ""},
    {"vary", ""},
    {"via", ""},
    {"www-authenticate", ""},
};

static int getint(uint8_t **rbuf, uint8_t mask)
{
	uint8_t cur = (*rbuf)[0];
	int shift = 0;

	if ((cur & mask) != mask) {
		(*rbuf)++;
		return cur & mask;
	}

	do {
		cur = *(++(*rbuf));
		mask += ((cur & 0x7F) << (shift));
		shift += 7;
	} while ((cur & 0x80));

	(*rbuf)++;
	return mask;
}

static void writeint(uint8_t **wbuf, int num, uint8_t prefix, uint8_t mask)
{
	if (num < mask) {
		**wbuf = (uint8_t)(prefix | num);
		(*wbuf)++;
	} else {
		**wbuf = (uint8_t)(prefix | mask);
		(*wbuf)++;
		num -= (prefix | mask);

		while (num) {
			**wbuf = (((num > 128) ? 0x80 : 0x00) + num % 128);
			(*wbuf)++;
			num >>= 7;
		}
	}
}

static void recv_header_kv(uint8_t **rbuf, char *buf, size_t buflen)
{
	int hdecoded = ((*rbuf)[0] & 0x80);
	int vlen = getint(rbuf, 0x7F);
	if (hdecoded)
		huffman_decode(*rbuf, vlen, (uint8_t *)buf, buflen);
	else
		memcpy(buf, *rbuf, vlen);
	*rbuf += vlen;
}

void hpack_header_decode(hdt *dytbl, struct h2o_hashmap *headers,
			 uint8_t **rbuf)
{
	int idx, ret = 0;
	char key[256] = {}, value[256] = {};
	size_t keylen = sizeof(key), valuelen = sizeof(value);
	uint8_t b0 = (*rbuf)[0];

	/*
	 * Maximum Dynamic Table Size Change
	 */
	if ((b0 & 0xE0) == 0x20) {
		int sz = getint(rbuf, 0x1F);
		printf("Maximum Dynamic Table Size Change: %d\n", sz);
		hdt_update_size(dytbl, sz);
		return;
	}

	/*
	 * Indexed Header Field Representation
	 */
	if ((b0 & 0x80) == 0x80) {
		idx = getint(rbuf, 0x7F);
		if (idx <= ST_NUM_OF_ENTRIES) {
			h2o_hashmap_set(headers, GET_ST_NAME(idx),
					GET_ST_VALUE(idx));
			return;
		}

		ret = hdt_peek_nth(dytbl, key, &keylen, value, &valuelen,
				   idx - ST_NUM_OF_ENTRIES - 1);
		if (ret)
			printf("failed to peek element from HDT, "
			       "index: %d, ret: %d\n",
			       idx, ret);
	}
	/*
	 * Literal Header Field With Incremental Indexing -- New Name
	 */
	else if (b0 == 0x40) {
		(*rbuf)++;
		recv_header_kv(rbuf, key, sizeof(key));
		recv_header_kv(rbuf, value, sizeof(value));
		ret = hdt_push(dytbl, key, strlen(key), value, strlen(value));
		if (ret)
			printf("Failed to push to HDT..., ret: %d\n", ret);
	}
	/*
	 * Literal Header Field With Incremental Indexing -- Indexed Name
	 */
	else if ((b0 & 0xC0) == 0x40) {
		idx = getint(rbuf, 0x3F);
		if (idx <= ST_NUM_OF_ENTRIES)
			memcpy(key, GET_ST_NAME(idx), strlen(GET_ST_NAME(idx)));
		else
			ret = hdt_peek_nth(dytbl, key, &keylen, NULL, NULL,
					   idx - ST_NUM_OF_ENTRIES - 1);
		if (ret)
			printf("failed to peek element from HDT, "
			       "index: %d, ret: %d\n",
			       idx, ret);
		recv_header_kv(rbuf, value, sizeof(value));
		ret = hdt_push(dytbl, key, strlen(key), value, strlen(value));
		if (ret)
			printf("Failed to push to HDT, ret: %d\n", ret);
	}
	/*
	 * Literal Header Field Without Indexing -- New Name
	 */
	else if (b0 == 0x00) {
		printf("Literal Header Field Without Indexing -- New Name\n");
		(*rbuf)++;
		recv_header_kv(rbuf, key, sizeof(key));
		recv_header_kv(rbuf, value, sizeof(value));
	}
	/*
	 * Literal Header Field Without Indexing -- Indexed Name
	 */
	else if ((b0 & 0xF0) == 0x00) {
		idx = getint(rbuf, 0x0F);
		if (idx <= ST_NUM_OF_ENTRIES)
			memcpy(key, GET_ST_NAME(idx), strlen(GET_ST_NAME(idx)));
		else
			ret = hdt_peek_nth(dytbl, key, &keylen, NULL, NULL,
					   idx - ST_NUM_OF_ENTRIES - 1);
		if (ret)
			printf("failed to peek element from HDT, "
			       "index: %d, ret: %d\n",
			       idx, ret);
		recv_header_kv(rbuf, value, sizeof(value));
	}
	/*
	 * Literal Header Field Never Indexed -- New Name
	 */
	else if (b0 == 0x10) {
		printf("Literal Header Field Never Indexed -- New Name\n");
		(*rbuf)++;
		recv_header_kv(rbuf, key, sizeof(key));
		recv_header_kv(rbuf, value, sizeof(value));
	}
	/*
	 * Literal Header Field Never Indexed -- Indexed Name
	 */
	else if ((b0 & 0xF0) == 0x10) {
		printf("Literal Header Field Never Indexed -- Indexed Name\n");
		idx = getint(rbuf, 0x0F);
		if (idx <= ST_NUM_OF_ENTRIES)
			memcpy(key, GET_ST_NAME(idx), strlen(GET_ST_NAME(idx)));
		else
			ret = hdt_peek_nth(dytbl, key, &keylen, NULL, NULL,
					   idx - ST_NUM_OF_ENTRIES - 1);
		if (ret)
			printf("failed to peek element from HDT, "
			       "index: %d, ret: %d\n",
			       idx, ret);
		recv_header_kv(rbuf, value, sizeof(value));
	}
	h2o_hashmap_set(headers, key, value);
}

int hpack_header_encode(hdt *dytbl, uint8_t **wbuf, char *name, size_t namelen,
			char *value, size_t valuelen)
{
	for (int i = 1; i <= ST_NUM_OF_ENTRIES; i++) {
		if (strncmp(GET_ST_NAME(i), name, namelen) == 0) {
			if (strncmp(GET_ST_VALUE(i), value, valuelen) == 0) {
				writeint(wbuf, i, 0x80, 0x7F);
			} else {
				writeint(wbuf, i, 0x40, 0x3F);
				// Huffman encoded?
				writeint(wbuf, valuelen, 0x00, 0x7F);
				memcpy(*wbuf, value, valuelen);
				(*wbuf) += valuelen;
			}
			return 0;
		}
	}

	hdt_entry *data = NULL;
	int idx = ST_NUM_OF_ENTRIES + 1;
	hdt_for_each_entry_reverse(data, &dytbl->head, list)
	{
		if (strncmp((char *)(data->buf), name, namelen)) {
			if (strncmp((char *)(data->buf + data->namelen), value,
				    valuelen) == 0)
				writeint(wbuf, idx, 0x80, 0x7F);
			else {
				writeint(wbuf, idx, 0x40, 0x3F);
				// Huffman encoded?
				writeint(wbuf, valuelen, 0x00, 0x7F);
				memcpy(*wbuf, value, valuelen);
				(*wbuf) += valuelen;
			}
			return 0;
		}
		idx++;
	}
	return -ENOTSUP;
}

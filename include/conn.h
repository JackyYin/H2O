#ifndef _H2O_CONN_H
#define _H2O_CONN_H

#include "hashmap.h"
#include "hdt.h"
#include "state_machine.h"
#include <stdint.h>

struct h2o_stream {
	// uint32_t stream_id;
	struct h2o_hashmap *headers;
	h2o_ssm sm;
};

struct h2o_stream_map {
	int capacity;
	struct h2o_stream *streams;
};

struct h2o_conn_context {
	int negotiated;
	int fd;
	int pipefds[2];
	/*
	 * See: `h2o_settings`
	 */
	unsigned int settings[H2O_SETTINGS_MAX];
	hdt dytbl;
	/*
	 * Maintain multiple stream status in a connection
	 */
	int max_stream_id;
	struct h2o_stream_map stream_map;
	uint8_t wbufs[1024];
	uint8_t *writebuf;
};

#define GET_CONN_WRITE_SZ(ctx) (size_t)(ctx->writebuf - ctx->wbufs)

struct h2o_conn_context *h2o_conn_context_init();
void h2o_conn_context_uninit(struct h2o_conn_context *ctx);

struct h2o_stream *h2o_stream_map_get_stream(struct h2o_stream_map *map,
					     int stream_id);
int h2o_stream_map_maybe_extend(struct h2o_stream_map *map, int stream_id);
#endif

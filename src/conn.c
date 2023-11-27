#include "conn.h"
#include "h2o.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_STREAM_MAP_SZ (8)

static int h2o_stream_map_init(struct h2o_stream_map *map)
{
	map->capacity = DEFAULT_STREAM_MAP_SZ;

	map->streams = malloc(sizeof(struct h2o_stream) * map->capacity);
	if (!map->streams)
		return -ENOMEM;

	memset(map->streams, 0, sizeof(struct h2o_stream) * map->capacity);
	return 0;
}

static int h2o_stream_map_uninit(struct h2o_stream_map *map)
{
	free(map->streams);
	map->streams = NULL;
	return 0;
}

struct h2o_stream *h2o_stream_map_get_stream(struct h2o_stream_map *map,
					     int stream_id)
{
	if (stream_id >= map->capacity)
		return NULL;

	return &map->streams[stream_id];
}

int h2o_stream_map_maybe_extend(struct h2o_stream_map *map, int stream_id)
{
	// extend stream map
	if (stream_id >= map->capacity) {
		struct h2o_stream *new =
		    realloc(map->streams,
			    sizeof(struct h2o_stream) * map->capacity * 2);
		if (!new)
			return -ENOMEM;

		map->streams = new;
		map->capacity *= 2;
		return 0;
	}
	return -EAGAIN;
}

struct h2o_conn_context *h2o_conn_context_init()
{
	struct h2o_conn_context *ctx = malloc(sizeof(struct h2o_conn_context));
	if (ctx) {
		memset(ctx, 0, sizeof(struct h2o_conn_context));
		ctx->settings[H2O_SETTINGS_HEADER_TABLE_SIZE] = 4096;
		ctx->settings[H2O_SETTINGS_ENABLE_PUSH] = 0;
		ctx->settings[H2O_SETTINGS_INITIAL_WINDOW_SIZE] = 65535;
		ctx->settings[H2O_SETTINGS_MAX_FRAME_SIZE] = 16384;

		// init dynamic table
		hdt_init(&ctx->dytbl,
			 ctx->settings[H2O_SETTINGS_HEADER_TABLE_SIZE]);

		// init stream map
		if (h2o_stream_map_init(&ctx->stream_map) < 0) {
			free(ctx);
			return NULL;
		}

		// init pipe fds
		if ((pipe(ctx->pipefds)) < 0) {
			h2o_stream_map_uninit(&ctx->stream_map);
			free(ctx);
			return NULL;
		}
	}
	return ctx;
}

void h2o_conn_context_uninit(struct h2o_conn_context *ctx)
{
	close(ctx->pipefds[0]);
	close(ctx->pipefds[1]);
	h2o_stream_map_uninit(&ctx->stream_map);
	free(ctx);
}

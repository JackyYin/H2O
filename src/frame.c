#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conn.h"
#include "frame.h"
#include "h2o.h"
#include "hashmap.h"
#include "hpack.h"
#include "router.h"

static uint8_t read_buf_be_8(uint8_t **buf)
{
	uint8_t val = (*buf)[0];
	*buf += 1;
	return val;
}

static uint8_t read_buf_be_16(uint8_t **buf)
{
	uint8_t val = ((*buf)[0] << 8) | ((*buf)[1]);
	*buf += 2;
	return val;
}

static uint32_t read_buf_be_24(uint8_t **buf)
{
	uint32_t val = ((*buf)[0] << 16) | ((*buf)[1] << 8) | ((*buf)[2]);
	*buf += 3;
	return val;
}

static uint32_t read_buf_be_32(uint8_t **buf)
{
	uint32_t val = ((*buf)[0] << 24) | ((*buf)[1] << 16) |
		       ((*buf)[2] << 8) | ((*buf)[3]);
	*buf += 4;
	return val;
}

static void write_buf_be_8(uint8_t **buf, uint8_t val)
{
	(*buf)[0] = val;
	*buf += 1;
}

static void write_buf_be_16(uint8_t **buf, uint16_t val)
{
	(*buf)[0] = (val >> 8) & 0xFF;
	(*buf)[1] = (val & 0xFF);
	*buf += 2;
}

static void write_buf_be_24(uint8_t **buf, uint32_t val)
{
	(*buf)[0] = (val >> 16) & 0xFF;
	(*buf)[1] = (val >> 8) & 0xFF;
	(*buf)[2] = (val & 0xFF);
	*buf += 3;
}

static void write_buf_be_32(uint8_t **buf, uint32_t val)
{
	(*buf)[0] = (val >> 24) & 0xFF;
	(*buf)[1] = (val >> 16) & 0xFF;
	(*buf)[2] = (val >> 8) & 0xFF;
	(*buf)[3] = (val & 0xFF);
	*buf += 4;
}

void compose_frame_header(uint8_t **buf, uint32_t length, uint8_t type,
			  uint8_t flags, uint32_t stream_id)
{
	// length
	write_buf_be_24(buf, length);
	// type
	write_buf_be_8(buf, type);
	// flags
	write_buf_be_8(buf, flags);
	// stream ID
	write_buf_be_32(buf, stream_id);
}

void compose_goaway_frame(struct h2o_conn_context *ctx, uint8_t **buf,
			  uint32_t err)
{
	// length
	write_buf_be_24(buf, 8);
	// type
	write_buf_be_8(buf, H2O_FRAME_TYPE_GOAWAY);
	// flags
	write_buf_be_8(buf, 0);
	// stream ID
	write_buf_be_32(buf, 0);
	// last stream ID
	write_buf_be_32(buf, ctx->max_stream_id);
	// error code
	write_buf_be_32(buf, err);
}

void compose_settings_frame(struct h2o_conn_context *ctx, uint8_t **buf)
{
	// length
	write_buf_be_24(buf, 6);
	// type
	write_buf_be_8(buf, H2O_FRAME_TYPE_SETTINGS);
	// flags
	write_buf_be_8(buf, 0);
	// stream ID
	write_buf_be_32(buf, 0);

	// settings
	write_buf_be_16(buf, H2O_SETTINGS_HEADER_TABLE_SIZE);
	write_buf_be_32(buf, ctx->settings[H2O_SETTINGS_HEADER_TABLE_SIZE]);
	printf("composed settings frame...\n");
}

void compose_settings_ack_frame(uint8_t **buf)
{
	// length
	write_buf_be_24(buf, 0);
	// type
	write_buf_be_8(buf, H2O_FRAME_TYPE_SETTINGS);
	// flags
	write_buf_be_8(buf, ACK_FLAG);
	// stream ID
	write_buf_be_32(buf, 0);

	printf("composed settings ACK frame...\n");
}

int compose_headers_frame(struct h2o_conn_context *ctx, uint8_t **buf,
			  uint8_t flags, uint32_t stream_id, char *type)
{
	struct h2o_stream *stream =
	    h2o_stream_map_get_stream(&ctx->stream_map, stream_id);

	if (!stream) {
		printf("WARNING: stream not found for stream ID: %u\n",
		       stream_id);
		return -ENOENT;
	}

	int err = ssm_check_send_frame(&stream->sm, H2O_FRAME_TYPE_HEADERS);
	if (err)
		return err;

	handle_external_event(&stream->sm, SSM_EVENT_SEND_H);
	if (flags & END_STREAM_FLAG) {
		handle_external_event(&stream->sm, SSM_EVENT_SEND_ES);
	}

	uint8_t *field_buf = *buf + 9;
	// field block fragment
	hpack_header_encode(&ctx->dytbl, &field_buf, ":status",
			    sizeof(":status"), "200", sizeof("200"));

	hpack_header_encode(&ctx->dytbl, &field_buf, "content-type",
			    sizeof("content-type"), type, strlen(type));
	// length
	write_buf_be_24(buf, field_buf - *buf - 9);
	// type
	write_buf_be_8(buf, H2O_FRAME_TYPE_HEADERS);
	// flags
	write_buf_be_8(buf, flags);
	// stream ID
	write_buf_be_32(buf, stream_id);

	*buf = field_buf;
	return 0;
}

int compose_rst_stream_frame(struct h2o_conn_context *ctx, uint8_t **buf,
			     uint32_t stream_id, uint32_t error_code)
{
	struct h2o_stream *stream =
	    h2o_stream_map_get_stream(&ctx->stream_map, stream_id);

	if (!stream) {
		printf("WARNING: stream not found for stream ID: %u\n",
		       stream_id);
		return -ENOENT;
	}

	int err = ssm_check_send_frame(&stream->sm, H2O_FRAME_TYPE_RST_STREAM);
	if (err)
		return err;

	handle_external_event(&stream->sm, SSM_EVENT_SEND_R);

	// length
	write_buf_be_24(buf, 4);
	// type
	write_buf_be_8(buf, H2O_FRAME_TYPE_RST_STREAM);
	// flags
	write_buf_be_8(buf, 0);
	// stream ID
	write_buf_be_32(buf, stream_id);
	//  error code
	write_buf_be_32(buf, error_code);
	return 0;
}

static void data_frame_parser(uint32_t length, uint8_t flags,
			      uint32_t stream_id, uint8_t **rbuf,
			      uint8_t **wbuf, struct h2o_conn_context *ctx)
{
	struct h2o_stream *stream =
	    h2o_stream_map_get_stream(&ctx->stream_map, stream_id);

	if (flags & END_STREAM_FLAG) {
		handle_external_event(&stream->sm, SSM_EVENT_RECV_ES);
	}

	uint8_t *rbufend = *rbuf + length;
	uint8_t padlen = 0;
	// PADDED flag
	if (flags & PADDED_FLAG) {
		padlen = read_buf_be_8(rbuf) + 1; // 8 bits for padlen itself
	}

	char tmp[64] = {};
	memcpy(tmp, *rbuf, length - padlen);
	printf("data frame data: %s\n", tmp);

	*rbuf = rbufend;
}

static void header_frame_parser(uint32_t length, uint8_t flags,
				uint32_t stream_id, uint8_t **rbuf,
				uint8_t **wbuf, struct h2o_conn_context *ctx)
{
	uint8_t padlen = 0;
	uint32_t stream_dep = 0;
	uint8_t weight = 0;
	if (flags & PADDED_FLAG)
		padlen = read_buf_be_8(rbuf);

	if (flags & PRIORITY_FLAG) {
		stream_dep = read_buf_be_32(rbuf);
		weight = read_buf_be_8(rbuf);
	}

	printf("header PADDING length: %u\n", padlen);
	printf("header PRIORITY exclusive: %u stream_dep: %u, weight: "
	       "%u\n",
	       stream_dep & 0x80000000 ? 1 : 0, stream_dep & 0x7FFFFFFF,
	       weight);

	struct h2o_stream *stream =
	    h2o_stream_map_get_stream(&ctx->stream_map, stream_id);

	handle_external_event(&stream->sm, SSM_EVENT_RECV_H);
	if (flags & END_STREAM_FLAG) {
		handle_external_event(&stream->sm, SSM_EVENT_RECV_ES);
	}

	if (!stream->headers)
		stream->headers = h2o_hashmap_init();

	uint8_t *rbufend = *rbuf + length;
	while (*rbuf < rbufend) {
		hpack_header_decode(&ctx->dytbl, stream->headers, rbuf);
	}
	*rbuf = rbufend;
}

static void rst_stream_frame_parser(uint32_t length, uint8_t flags,
				    uint32_t stream_id, uint8_t **rbuf,
				    uint8_t **wbuf,
				    struct h2o_conn_context *ctx)
{
	uint32_t err = read_buf_be_32(rbuf);
	printf("receive rst_stream frame error code: %d\n", err);
}

static void settings_frame_parser(uint32_t length, uint8_t flags,
				  uint32_t stream_id, uint8_t **rbuf,
				  uint8_t **wbuf, struct h2o_conn_context *ctx)
{
	uint8_t *rbufend = *rbuf + length;

	while (*rbuf < rbufend) {
		uint16_t key = read_buf_be_16(rbuf);
		uint32_t val = read_buf_be_32(rbuf);

		printf("setting key : %d, val: %d\n", key, val);

		if (key > 0 && key < H2O_SETTINGS_MAX)
			ctx->settings[key] = val;

		if (key == H2O_SETTINGS_HEADER_TABLE_SIZE)
			hdt_update_size(
			    &ctx->dytbl,
			    ctx->settings[H2O_SETTINGS_HEADER_TABLE_SIZE]);
	}
}

static void goaway_frame_parser(uint32_t length, uint8_t flags,
				uint32_t stream_id, uint8_t **rbuf,
				uint8_t **wbuf, struct h2o_conn_context *ctx)
{
	uint8_t *rbufend = *rbuf + length;
	uint32_t last_stream_id = read_buf_be_32(rbuf);
	uint32_t err = read_buf_be_32(rbuf);

	printf("receive goaway last_stream_id: %d, err: %d\n", last_stream_id,
	       err);
	printf("%s\n", *rbuf);
	*rbuf = rbufend;
}

static void window_update_frame_parser(uint32_t length, uint8_t flags,
				       uint32_t stream_id, uint8_t **rbuf,
				       uint8_t **wbuf,
				       struct h2o_conn_context *ctx)
{
	uint32_t val = read_buf_be_32(rbuf);
	printf("window update val: %d\n", val);
}

static void (*frame_parser_table[])(uint32_t, uint8_t, uint32_t, uint8_t **,
				    uint8_t **, struct h2o_conn_context *) = {
    [H2O_FRAME_TYPE_DATA] = data_frame_parser,
    [H2O_FRAME_TYPE_HEADERS] = header_frame_parser,
    [H2O_FRAME_TYPE_RST_STREAM] = rst_stream_frame_parser,
    [H2O_FRAME_TYPE_SETTINGS] = settings_frame_parser,
    [H2O_FRAME_TYPE_GOAWAY] = goaway_frame_parser,
    [H2O_FRAME_TYPE_WINDOW_UPDATE] = window_update_frame_parser,
};

void frame_ssm_cb(h2o_ssm_state new_state, void *data)
{
	struct h2o_conn_context *ctx = (struct h2o_conn_context *)data;

	switch (new_state) {
	case SSM_STATE_ERR_PROTOCOL_ERROR:
		compose_goaway_frame(ctx, &ctx->writebuf,
				     H2O_ERROR_CODE_PROTOCOL_ERROR);
		break;
	case SSM_STATE_ERR_STREAM_CLOSED_ERROR:
		compose_goaway_frame(ctx, &ctx->writebuf,
				     H2O_ERROR_CODE_STREAM_CLOSED_ERROR);
		break;
	default:
		break;
	}
}

void parse_frame(struct h2o *h, struct h2o_conn_context *ctx, uint8_t **rbuf,
		 uint8_t **wbuf)
{
	uint32_t length = read_buf_be_24(rbuf);
	uint8_t type = read_buf_be_8(rbuf);
	uint8_t flags = read_buf_be_8(rbuf);
	uint32_t stream_id = read_buf_be_32(rbuf);

	printf("frame length: %d, type: %d, flags: %d, sid: %d\n", length, type,
	       flags, stream_id);

	// update max stream ID
	if (stream_id > ctx->max_stream_id)
		ctx->max_stream_id = stream_id;

	h2o_stream_map_maybe_extend(&ctx->stream_map, stream_id);

	struct h2o_stream *stream =
	    h2o_stream_map_get_stream(&ctx->stream_map, stream_id);

	if (!stream) {
		printf("FATAL: stream not found for stream ID: %u\n",
		       stream_id);
		exit(-1);
	}

	h2o_ssm *m = &stream->sm;
	if (!m->state_action_cb) {
		init_ssm(m);
		register_state_action_callback(m, frame_ssm_cb, (void *)ctx);
	}

	if (frame_parser_table[type])
		frame_parser_table[type](length, flags, stream_id, rbuf, wbuf,
					 ctx);

	if (stream->headers) {
		char *path = h2o_hashmap_get(stream->headers, ":path");
		char *foundpath = NULL;
		struct h2o_router *router =
		    h2o_trie_search(&h->url_map_trie, path, &foundpath, 1);

		if (router) {
			router->handler(h, ctx, router, stream_id,
					router->index_html);
		} else {
			router = h2o_trie_search(&h->url_map_trie, path,
						 &foundpath, 0);
			if (router) {
				router->handler(h, ctx, router, stream_id,
						path + strlen(foundpath));
			}
		}

		if (flags & END_HEADERS_FLAG) {
			h2o_hashmap_uninit(stream->headers);
			stream->headers = NULL;
		}
	}
}

#ifndef _H2O_FRAME_H
#define _H2O_FRAME_H

#include <stdint.h>

#define END_STREAM_FLAG (0x01)
#define ACK_FLAG (0x01)
#define END_HEADERS_FLAG (0x04)
#define PADDED_FLAG (0x08)
#define PRIORITY_FLAG (0x20)

struct h2o_conn_context;

void compose_frame_header(uint8_t **buf, uint32_t length, uint8_t type,
			  uint8_t flags, uint32_t stream_id);

void compose_goaway_frame(struct h2o_conn_context *ctx, uint8_t **buf,
			  uint32_t err);

void compose_settings_frame(struct h2o_conn_context *ctx, uint8_t **buf);
void compose_settings_ack_frame(uint8_t **buf);

int compose_headers_frame(struct h2o_conn_context *ctx, uint8_t **buf,
			  uint8_t flag, uint32_t stream_id, char *type);

void parse_frame(struct h2o *h, struct h2o_conn_context *ctx, uint8_t **rbuf,
		 uint8_t **wbuf);

#endif

#ifndef _H2O_H
#define _H2O_H

#include <stdint.h>

/**
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-4.1
 */
struct h2o_frame_header {
	/**
	 * Unsigned 24-bit integer, The 9 octets of the frame header are not
	 * included in this value.
	 */
	uint32_t length;

	/**
	 * See: `h2_frame_type`
	 */
	uint8_t type;

	uint8_t flags;

	uint32_t reserved : 1;

	uint32_t stream_id : 31;
};

/**
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#name-frame-definitions
 */
enum h2o_frame_type {
	H2O_FRAME_TYPE_DATA,
	H2O_FRAME_TYPE_HEADER,
	H2O_FRAME_TYPE_PRIORITY,
	H2O_FRAME_TYPE_RST_STREAM,
	H2O_FRAME_TYPE_SETTINGS,
	H2O_FRAME_TYPE_PUSH_PROMISE,
	H2O_FRAME_TYPE_PING,
	H2O_FRAME_TYPE_GOAWAY,
	H2O_FRAME_TYPE_WINDOW_UPDATE,
	H2O_FRAME_TYPE_CONTINUATION
};

/**
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-5.1
 */
enum h2o_stream_state {
	H2O_STREAM_STATE_IDLE,
	H2O_STREAM_STATE_RESERVED_LOCAL,
	H2O_STREAM_STATE_RESERVED_REMOTE,
	H2O_STREAM_STATE_OPEN,
	H2O_STREAM_STATE_HALF_CLOSED_LOCAL,
	H2O_STREAM_STATE_HALF_CLOSED_REMOTE,
	H2O_STREAM_STATE_CLOSED
};

#endif

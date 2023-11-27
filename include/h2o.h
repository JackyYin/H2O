#ifndef _H2O_H
#define _H2O_H

#include <liburing.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>
#include <mbedtls/x509.h>
#include <stdint.h>

#include "trie.h"
/*
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#name-frame-definitions
 */
enum h2o_frame_type {
	H2O_FRAME_TYPE_DATA,
	H2O_FRAME_TYPE_HEADERS,
	H2O_FRAME_TYPE_PRIORITY,
	H2O_FRAME_TYPE_RST_STREAM,
	H2O_FRAME_TYPE_SETTINGS,
	H2O_FRAME_TYPE_PUSH_PROMISE,
	H2O_FRAME_TYPE_PING,
	H2O_FRAME_TYPE_GOAWAY,
	H2O_FRAME_TYPE_WINDOW_UPDATE,
	H2O_FRAME_TYPE_CONTINUATION,
};

/*
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

/*
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-6.5.2
 */
enum h2o_settings {
	H2O_SETTINGS_HEADER_TABLE_SIZE = 0x1,
	H2O_SETTINGS_ENABLE_PUSH = 0x2,
	H2O_SETTINGS_MAX_CONCURRENT_STREAMS = 0x3,
	H2O_SETTINGS_INITIAL_WINDOW_SIZE = 0x4,
	H2O_SETTINGS_MAX_FRAME_SIZE = 0x5,
	H2O_SETTINGS_HEADER_LIST_SIZE = 0x6,
	H2O_SETTINGS_MAX,
};

/*
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-7
 */
enum h2o_error_code {
	H2O_ERROR_CODE_NO_ERROR,
	H2O_ERROR_CODE_PROTOCOL_ERROR,
	H2O_ERROR_CODE_INTERNAL_ERROR,
	H2O_ERROR_CODE_FLOW_CONTROL_ERROR,
	H2O_ERROR_CODE_SETTINGS_TIMEOUT_ERROR,
	H2O_ERROR_CODE_STREAM_CLOSED_ERROR,
	H2O_ERROR_CODE_FRAME_SIZE_ERROR,
	H2O_ERROR_CODE_REFUSED_STREAM_ERROR,
	H2O_ERROR_CODE_CANCEL,
	H2O_ERROR_CODE_COMPRESSION_ERROR,
	H2O_ERROR_CODE_CONNECT_ERROR,
	H2O_ERROR_CODE_ENHANCE_YOUR_CALM,
	H2O_ERROR_CODE_INADEQUATE_SECURITY,
	H2O_ERROR_CODE_HTTP_1_1_REQUIRED,
};

/*
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-4.1
 */
struct h2o_frame_header {
	/**
	 * The 9 octets of the frame header are not
	 * included in this value.
	 */
	uint32_t length : 24;

	/**
	 * See: `h2o_frame_type`
	 */
	uint32_t type : 8;

	uint8_t flags;

	uint32_t reserved : 1;

	uint32_t stream_id : 31;
} __attribute__((packed));

/*
 * Ref: https://www.rfc-editor.org/rfc/rfc9113#section-6.5.1
 */
struct h2o_setting {
	uint16_t id;
	uint32_t value;
} __attribute__((packed));

struct h2o {
	int server_fd;
	struct io_uring ring;
	struct h2o_trie url_map_trie;

	struct {
		mbedtls_ssl_config conf;
		mbedtls_x509_crt cert;
		mbedtls_pk_context pkey;
		mbedtls_ctr_drbg_context ctr_drbg;
		mbedtls_entropy_context entropy;
	} tls;
};
#endif

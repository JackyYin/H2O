#include <arpa/inet.h>
#include <errno.h>
#include <linux/tcp.h>
#include <linux/tls.h>
#include <mbedtls/chacha20.h>
#include <mbedtls/chachapoly.h>
#include <mbedtls/gcm.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_internal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "conn.h"
#include "frame.h"
#include "h2o.h"
#include "iouring.h"

#define MAX_MESSAGE_LEN 2048
#define BUFFERS_COUNT ((1 << 14) / MAX_MESSAGE_LEN)

#define CONN_PREFACE "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"

#define STRLEN(str) (sizeof(str) - 1)
#define MATCH_PREFIX(arg, str) (strncmp(arg, str, STRLEN(str)) == 0)

static uint8_t bufs[BUFFERS_COUNT][MAX_MESSAGE_LEN] = {0};
static uint16_t bgid = 1024;

static int setup_listening_socket(int port)
{
	int sock, enable = 1;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -errno;

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) ==
	    -1)
		goto CLOSE_ERR;

	struct sockaddr_in srv_addr = {.sin_family = AF_INET,
				       .sin_port = htons(port),
				       .sin_addr.s_addr = htonl(INADDR_ANY)};

	/* We bind to a port and turn this socket into a listening
	 * socket.
	 * */
	if (bind(sock, (const struct sockaddr *)&srv_addr, sizeof(srv_addr)) ==
	    -1)
		goto CLOSE_ERR;

	if (listen(sock, 10) == -1)
		goto CLOSE_ERR;

	return (sock);

CLOSE_ERR:
	close(sock);
	return -errno;
}

static int provide_buf_request(struct io_uring *ring, int gid)
{
	struct io_uring_cqe *cqe;
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	io_uring_prep_provide_buffers(sqe, bufs, MAX_MESSAGE_LEN, BUFFERS_COUNT,
				      gid, 0);
	io_uring_submit_and_wait(ring, 1);

	io_uring_peek_cqe(ring, &cqe);
	if (cqe->res < 0)
		return cqe->res;
	io_uring_cqe_seen(ring, cqe);
	return 0;
}

struct h2o_exp_key {
	unsigned char master_secret[48];
	unsigned char random_bytes[64];
	unsigned char iv[32];
	unsigned char key[32];
};

static int h2o_mbedtls_send(void *ctx, const unsigned char *buf, size_t len)
{
	int *fd = (int *)ctx;
	return send(*fd, buf, len, 0);
}
static int h2o_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{

	int *fd = (int *)ctx;
	return recv(*fd, buf, len, 0);
}

#ifndef SOL_TCP
#define SOL_TCP 6
#endif

static int setup_tls_keys(int fd, const mbedtls_ssl_context *ssl, int tx_or_rx)
{
	/* struct tls12_crypto_info_aes_gcm_128 crypto_info; */
	struct tls12_crypto_info_chacha20_poly1305 crypto_info;
	const unsigned char *iv, *rec_seq;
	const mbedtls_chachapoly_context *chachapoly_ctx;
	/* const mbedtls_chacha20_context *chacha_ctx; */
	/* const mbedtls_gcm_context *gcm_ctx; */
	/* const mbedtls_aes_context *aes_ctx; */

	switch (tx_or_rx) {
	case TLS_RX:
		iv = ssl->transform->iv_dec;
		rec_seq = ssl->in_ctr;
		chachapoly_ctx = ssl->transform->cipher_ctx_dec.cipher_ctx;
		break;
	case TLS_TX:
		iv = ssl->transform->iv_enc;
		rec_seq = ssl->cur_out_ctr;
		chachapoly_ctx = ssl->transform->cipher_ctx_enc.cipher_ctx;
		break;
	default:
		__builtin_unreachable();
	}

	/* iv = salt + 4; */
	/* aes_ctx = gcm_ctx->cipher_ctx.cipher_ctx; */
	/* chacha_ctx = &chachapoly_ctx->chacha20_ctx; */

	memset(&crypto_info, 0, sizeof(crypto_info));
	crypto_info.info.version = TLS_1_2_VERSION;
	crypto_info.info.cipher_type = TLS_CIPHER_CHACHA20_POLY1305;

	/* memcpy(crypto_info.salt, salt, TLS_CIPHER_AES_GCM_128_SALT_SIZE); */
	memcpy(crypto_info.iv, iv, TLS_CIPHER_CHACHA20_POLY1305_IV_SIZE);
	memcpy(crypto_info.key, &chachapoly_ctx->chacha20_ctx.state[4],
	       TLS_CIPHER_CHACHA20_POLY1305_KEY_SIZE);
	memcpy(crypto_info.rec_seq, rec_seq,
	       TLS_CIPHER_CHACHA20_POLY1305_REC_SEQ_SIZE);

	return setsockopt(fd, SOL_TLS, tx_or_rx, &crypto_info,
			  sizeof(crypto_info));
}

static int setup_tls(const struct h2o *h, struct h2o_conn_context *ctx)
{
	int ret = 0, fd = ctx->fd;
	mbedtls_ssl_context ssl;
	struct h2o_exp_key k;
	memset(&k, 0, sizeof(k));

	mbedtls_ssl_init(&ssl);
	ret = mbedtls_ssl_setup(&ssl, &h->tls.conf);
	if (ret != 0)
		return ret;

	// mbedtls_ssl_set_export_keys_cb(&ssl, h2o_mbedtls_export_key_cb, &k);
	mbedtls_ssl_set_bio(&ssl, &fd, h2o_mbedtls_send, h2o_mbedtls_recv,
			    NULL);

	while (true) {
		ret = mbedtls_ssl_handshake(&ssl);
		switch (ret) {
		case 0:
			printf("SSL handshake done...\n");
			goto enable_tls_ulp;
		case MBEDTLS_ERR_SSL_ASYNC_IN_PROGRESS:
		case MBEDTLS_ERR_SSL_CRYPTO_IN_PROGRESS:
		case MBEDTLS_ERR_SSL_WANT_READ:
		case MBEDTLS_ERR_SSL_WANT_WRITE:
			break;
		default:
			printf("unknown error code: %d\n", ret);
			goto fail;
		}
	}

enable_tls_ulp:
	if (setsockopt(fd, SOL_TCP, TCP_ULP, "tls", sizeof("tls")) < 0) {
		printf("failed to init TCP ULP... %d\n", errno);
		goto fail;
	}
	ret = setup_tls_keys(fd, &ssl, TLS_RX);
	if (ret) {
		printf("failed to setsockopt RX... %d\n", errno);
		goto fail;
	}
	ret = setup_tls_keys(fd, &ssl, TLS_TX);
	if (ret) {
		printf("failed to setsockopt TX... %d\n", errno);
		goto fail;
	}

fail:
	mbedtls_ssl_free(&ssl);
	return ret;
}

#define ERR_CODE_SUCCESS (0)
#define ERR_CODE_GOTO_CQE_SEEN (-1)
#define ERR_CODE_GOTO_RELOOP (-2)

static int
svr_loop_accept_handler(struct h2o *h, struct io_uring_cqe *cqe,
			__attribute__((unused)) struct h2o_conn_context *ctx)
{
	struct io_uring *ring = &h->ring;

	fprintf(stdout, "accept res: %d, more : %d\n", cqe->res,
		cqe->flags & IORING_CQE_F_MORE);

	if (!(cqe->flags & IORING_CQE_F_MORE)) {
		add_accept_request(ring, h->server_fd, NULL);
	}

	if (cqe->res < 0) {
		fprintf(stderr, "Failed to accept: %d\n", cqe->res);
		return ERR_CODE_GOTO_CQE_SEEN;
	}

	struct h2o_conn_context *newctx = h2o_conn_context_init();
	if (!newctx) {
		fprintf(stderr, "Fatal Error: Failed to allocate!\n");
		close(cqe->res);
		return ERR_CODE_GOTO_CQE_SEEN;
	}
	newctx->fd = cqe->res;
	setup_tls(h, newctx);

	add_read_request(ring, cqe->res, newctx, bgid);
	return ERR_CODE_SUCCESS;
}

static int svr_loop_read_handler(struct h2o *h, struct io_uring_cqe *cqe,
				 struct h2o_conn_context *ctx)
{
	struct io_uring *ring = &h->ring;

	fprintf(stdout,
		"read fd: %d, res: %d, more : %d, "
		"nonempty: %d, "
		"bid: %d\n",
		ctx->fd, cqe->res, cqe->flags & IORING_CQE_F_MORE,
		cqe->flags & IORING_CQE_F_SOCK_NONEMPTY,
		cqe->flags >> IORING_CQE_BUFFER_SHIFT);

	if (cqe->res == -ENOBUFS) {
		io_uring_cqe_seen(ring, cqe);

		for (int bid = 0; bid < BUFFERS_COUNT; bid++) {
			add_buf_request(ring, (void *)bufs[bid],
					MAX_MESSAGE_LEN, bgid, bid,
					IOSQE_IO_LINK);
		}
		add_read_request(ring, ctx->fd, ctx, bgid);
		return ERR_CODE_GOTO_RELOOP;
	}

	/*
	 * cqe->res < 0 : error occured
	 * cqe->res == 0 : connection peer shutdown
	 */
	if (cqe->res <= 0) {
		fprintf(stderr, "Failed to read: %d\n", cqe->res);
		close(ctx->fd);
		h2o_conn_context_uninit(ctx);
		return ERR_CODE_GOTO_CQE_SEEN;
	}

	if (!(cqe->flags & IORING_CQE_F_BUFFER)) {
		fprintf(stderr, "IORING_CQE_F_BUFFER not set...\n");
		return ERR_CODE_GOTO_CQE_SEEN;
	}

	uint8_t *buf = bufs[cqe->flags >> IORING_CQE_BUFFER_SHIFT],
		*readbuf = buf, *readbufend = buf + cqe->res;
	ctx->writebuf = &ctx->wbufs[0];

	for (uint8_t *tmp = readbuf; tmp < readbufend; tmp++) {
		printf("%02X ", *tmp);
	}
	printf("\n");

	if (!ctx->negotiated) {
		if (!MATCH_PREFIX((char *)readbuf, CONN_PREFACE)) {
			printf("connection preface mismatched....\n");
			compose_goaway_frame(ctx, &ctx->writebuf,
					     H2O_ERROR_CODE_PROTOCOL_ERROR);
			add_write_request(ring, ctx->fd, ctx, ctx->wbufs,
					  (size_t)(GET_CONN_WRITE_SZ(ctx)),
					  IOSQE_IO_LINK);
			return ERR_CODE_GOTO_CQE_SEEN;
		}
		// server connection preface
		compose_settings_frame(ctx, &ctx->writebuf);
		ctx->negotiated = 1;
		readbuf += STRLEN(CONN_PREFACE);
	}

	while (readbuf < readbufend) {
		parse_frame(h, ctx, &readbuf, &ctx->writebuf);
	}

	if (GET_CONN_WRITE_SZ(ctx) > 0) {
		printf("write length: %ld\n", GET_CONN_WRITE_SZ(ctx));
		add_write_request(ring, ctx->fd, ctx, ctx->wbufs,
				  (size_t)(GET_CONN_WRITE_SZ(ctx)),
				  IOSQE_IO_LINK);
	}
	add_read_request(ring, ctx->fd, ctx, bgid);
	return ERR_CODE_SUCCESS;
}

static int svr_loop_write_handler(__attribute__((unused)) struct h2o *h,
				  struct io_uring_cqe *cqe,
				  struct h2o_conn_context *ctx)
{
	fprintf(stdout, "fd: %d, write res: %d\n", ctx->fd, cqe->res);
	return ERR_CODE_SUCCESS;
}

static int svr_loop_splice_handler(__attribute__((unused)) struct h2o *h,
				   struct io_uring_cqe *cqe,
				   struct h2o_conn_context *ctx)
{
	fprintf(stdout, "splice res: %d\n", cqe->res);
	return ERR_CODE_SUCCESS;
}

static int (*fd_state_handler[])(struct h2o *, struct io_uring_cqe *,
				 struct h2o_conn_context *) = {
    [IOURING_OP_TYPE_ACCEPT] = svr_loop_accept_handler,
    [IOURING_OP_TYPE_READ] = svr_loop_read_handler,
    [IOURING_OP_TYPE_WRITE] = svr_loop_write_handler,
    [IOURING_OP_TYPE_SPLICE] = svr_loop_splice_handler};

static int server_loop(struct h2o *h)
{
	struct io_uring *ring = &h->ring;
	struct io_uring_cqe *cqe;
	struct h2o_conn_context *ctx = NULL;
	add_accept_request(ring, h->server_fd, NULL);
	while (1) {
		/*
		 * Return number of submitted SQEs
		 */
		io_uring_submit_and_wait(ring, 1);

		if (io_uring_peek_cqe(ring, &cqe) < 0) {
			fprintf(stderr, "io_uring_peek_cqe failed...\n");
			goto RELOOP;
		}

		ctx = (struct h2o_conn_context *)h2o_cqe_get_data(cqe);
		int op_type = h2o_cqe_get_type(cqe);
		/*
		 * This cqe is from readd-buf request
		 *
		 * Note: We don't allocate cb for readd-buf request, because
		 * it's not necessary. We can save malloc/free time by doing
		 * this.
		 */
		if (op_type == IOURING_OP_TYPE_PROVIDE_BUF)
			goto CQE_SEEN;

		int ret = fd_state_handler[op_type](h, cqe, ctx);
		if (ret == ERR_CODE_GOTO_CQE_SEEN)
			goto CQE_SEEN;
		if (ret == ERR_CODE_GOTO_RELOOP)
			goto RELOOP;
	CQE_SEEN:
		/* Mark this request as processed */
		io_uring_cqe_seen(ring, cqe);
	RELOOP:
	}
	return 0;
}

int prepare_and_serve(struct h2o *h, int port)
{
	int ret = 0;

	ret = setup_listening_socket(port);
	if (ret < 0)
		return ret;
	h->server_fd = ret;

	ret = provide_buf_request(&h->ring, bgid);
	if (ret < 0)
		goto CLOSE;

	server_loop(h);
	ret = 0;

CLOSE:
	close(h->server_fd);
	return ret;
}

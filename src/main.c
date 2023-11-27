#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "h2o.h"

#define IOURING_QUEUE_DEPTH 256

static void parse_args(int argc, char **argv, struct h2o_config *conf)
{
	static struct option long_options[] = {
	    {"help", no_argument, 0, 'h'},
	    {"listen", required_argument, 0, 'l'},
	    {"config", required_argument, 0, 'c'}};

	const char *help_args = "\t-h\tdisplay help message\n"
				"\t-l\tspecify listening port\n"
				"\t-c\tspecify config file path\n";

	while (1) {
		int ret = getopt_long(argc, argv, "hl:c:", long_options, NULL);
		if (ret == -1)
			break;

		switch (ret) {
		case 'h':
			printf("Usage: %s\n%s\n", argv[0], help_args);
			exit(0);
		case 'l':
			conf->listen_on = atoi(optarg);
			break;
		case 'c':
			conf->config_file_path = strdup(optarg);
			break;
		case '?':
			break;
		default:
			printf("WARNING: getopt returned character code %d\n",
			       ret);
		}
	}
}

static int check_iouring_provide_buffers(struct io_uring *ring)
{
	struct io_uring_probe *probe = io_uring_get_probe_ring(ring);
	return probe &&
	       io_uring_opcode_supported(probe, IORING_OP_PROVIDE_BUFFERS);
}

static void my_debug(void *ctx, int level, const char *file, int line,
		     const char *str)
{
	fprintf((FILE *)ctx, "%s:%04d: %s\n", file, line, str);
	fflush((FILE *)ctx);
}

static int tls_init(struct h2o *h)
{
	static const int aes128_ciphers[] = {
	    /* Only allow Ephemeral Diffie-Hellman key exchange, so Perfect
	     * Forward Secrecy is possible.  */
	    MBEDTLS_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256,
	    MBEDTLS_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256,
	    /* MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256, */
	    /* MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, */
	    /* MBEDTLS_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256, */
	    /* MBEDTLS_TLS_DHE_PSK_WITH_AES_128_GCM_SHA256, */
	    0,
	};
	int ret = 0;
	mbedtls_ssl_config_init(&h->tls.conf);
	mbedtls_x509_crt_init(&h->tls.cert);
	mbedtls_pk_init(&h->tls.pkey);
	mbedtls_ctr_drbg_init(&h->tls.ctr_drbg);
	mbedtls_entropy_init(&h->tls.entropy);

	ret = mbedtls_ctr_drbg_seed(&h->tls.ctr_drbg, mbedtls_entropy_func,
				    &h->tls.entropy, NULL, 0);
	if (ret != 0)
		goto EXIT;

	ret = mbedtls_x509_crt_parse_file(&h->tls.cert, "./cert/cert.pem");
	if (ret != 0)
		goto EXIT;

	ret = mbedtls_pk_parse_keyfile(&h->tls.pkey, "./cert/key.pem", NULL);
	if (ret != 0)
		goto EXIT;

	ret = mbedtls_ssl_config_defaults(&h->tls.conf, MBEDTLS_SSL_IS_SERVER,
					  MBEDTLS_SSL_TRANSPORT_STREAM,
					  MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret != 0)
		goto EXIT;

	mbedtls_ssl_conf_renegotiation(&h->tls.conf,
				       MBEDTLS_SSL_RENEGOTIATION_DISABLED);
	mbedtls_ssl_conf_legacy_renegotiation(
	    &h->tls.conf, MBEDTLS_SSL_LEGACY_NO_RENEGOTIATION);
	mbedtls_ssl_conf_rng(&h->tls.conf, mbedtls_ctr_drbg_random,
			     &h->tls.ctr_drbg);
	mbedtls_ssl_conf_ciphersuites(&h->tls.conf, aes128_ciphers);
	mbedtls_ssl_conf_dbg(&h->tls.conf, my_debug, stdout);
	mbedtls_ssl_conf_ca_chain(&h->tls.conf, h->tls.cert.next, NULL);
	ret =
	    mbedtls_ssl_conf_own_cert(&h->tls.conf, &h->tls.cert, &h->tls.pkey);
	if (ret != 0)
		goto EXIT;

	static const char *alpn_protos[] = {"h2", NULL};
	ret = mbedtls_ssl_conf_alpn_protocols(&h->tls.conf, alpn_protos);
	if (ret != 0)
		goto EXIT;

	return ret;
EXIT:
	mbedtls_ssl_config_free(&h->tls.conf);
	mbedtls_x509_crt_free(&h->tls.cert);
	mbedtls_pk_free(&h->tls.pkey);
	mbedtls_ctr_drbg_free(&h->tls.ctr_drbg);
	mbedtls_entropy_free(&h->tls.entropy);
	return ret;
}

int h2o_init(struct h2o *h)
{
	int ret = 0;
	memset(h, 0, sizeof(*h));

	ret = tls_init(h);
	if (ret < 0)
		return ret;

	ret = io_uring_queue_init(IOURING_QUEUE_DEPTH, &h->ring, 0);
	if (ret < 0)
		return ret;

	if (!check_iouring_provide_buffers(&h->ring)) {
		ret = -ENOTSUP;
		goto EXIT_QUEUE;
	}

	return 0;
EXIT_QUEUE:
	io_uring_queue_exit(&h->ring);
	return ret;
}

int main(int argc, char **argv)
{
	struct h2o h;
	struct h2o_config conf = *h2o_get_default_config();

	if (h2o_init(&h))
		return -1;

	parse_args(argc, argv, &conf);
	h2o_init_config(&h, &conf);
	prepare_and_serve(&h, conf.listen_on);
}

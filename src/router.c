#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "frame.h"
#include "h2o.h"
#include "hashmap.h"
#include "iouring.h"
#include "router.h"

static char *get_mime(const char *path)
{
	char *type = NULL;
	if (strstr(path, ".htm"))
		type = "text/html";
	else if (strstr(path, ".js"))
		type = "text/javascript";
	return type;
}

static void h2o_router_handler(struct h2o *h, struct h2o_conn_context *ctx,
			       struct h2o_router *router, uint32_t stream_id,
			       char *path)
{
	printf("got h2o_router_handler path: %s\n", path);
	int fd = openat(router->root_fd, path, O_RDONLY);
	if (fd < 0) {
		printf("WARNING: route file not found...\n");
		return;
	}

	struct stat st;
	int ret = fstat(fd, &st);
	if (ret < 0) {
		close(fd);
		return;
	}

	printf("file fd: %d, size: %lu\n", fd, st.st_size);
	printf("pipe read end: %d, write end: %d\n", ctx->pipefds[0],
	       ctx->pipefds[1]);

	compose_headers_frame(ctx, &ctx->writebuf, END_HEADERS_FLAG, stream_id,
			      get_mime(path));
	compose_frame_header(&ctx->writebuf, st.st_size, H2O_FRAME_TYPE_DATA,
			     END_STREAM_FLAG, stream_id);
	add_write_request(&h->ring, ctx->fd, ctx, ctx->wbufs,
			  (size_t)(GET_CONN_WRITE_SZ(ctx)), IOSQE_IO_LINK);
	ctx->writebuf = &ctx->wbufs[0];
	/* file -> pipe write end */
	add_splice_request(&h->ring, fd, 0, ctx->pipefds[1], -1, st.st_size,
			   IOSQE_IO_LINK);
	/* pipe read end -> socket */
	add_splice_request(&h->ring, ctx->pipefds[0], -1, ctx->fd, -1,
			   st.st_size, IOSQE_IO_LINK);
}

struct h2o_router *h2o_init_router(struct h2o_hashmap *map)
{
	char realroot[256] = {0};
	char *root = h2o_hashmap_get(map, "root");
	char *index_html = h2o_hashmap_get(map, "index");

	if (!realpath(root, realroot)) {
		printf("WARNING: path %s invalid or not exist\n", root);
		return NULL;
	}

	int root_fd = open(realroot, O_RDONLY | O_DIRECTORY);
	if (root_fd < 0)
		return NULL;

	struct h2o_router *router = malloc(sizeof(*router));
	if (router) {
		router->root_fd = root_fd;
		/* Hashmap will be cleanup soon, duplicate string to another
		 * space for further usage
		 */
		router->index_html =
		    index_html ? strdup(index_html) : "index.html";
		router->handler = h2o_router_handler;
	}
	return router;
}

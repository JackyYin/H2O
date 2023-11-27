#ifndef _H2O_IOURING_H
#define _H2O_IOURING_H

#include <liburing.h>

#define IOURING_OP_TYPE_ACCEPT (1)
#define IOURING_OP_TYPE_READ (2)
#define IOURING_OP_TYPE_WRITE (3)
#define IOURING_OP_TYPE_SPLICE (4)
#define IOURING_OP_TYPE_PROVIDE_BUF (5)
#define IOURING_OP_TYPE_MASK (0x7UL)

#define h2o_cqe_get_data(cqe) (void *)((cqe)->user_data & ~IOURING_OP_TYPE_MASK)

#define h2o_cqe_get_type(cqe) (int)((cqe)->user_data & IOURING_OP_TYPE_MASK)

__attribute__((unused)) static int
add_accept_request(struct io_uring *ring, int server_socket, void *data)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	/* io_uring_prep_accept(sqe, server_socket, NULL, NULL, 0); */
	io_uring_prep_multishot_accept(sqe, server_socket, NULL, 0, 0);
	sqe->user_data = ((unsigned long)(data) | IOURING_OP_TYPE_ACCEPT);
	return 0;
}

__attribute__((unused)) static int
add_read_request(struct io_uring *ring, int client_socket, void *data, int gid)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	/* io_uring_prep_recv_multishot(sqe, client_socket, NULL, 0, 0); */
	io_uring_prep_recv(sqe, client_socket, NULL, 0, 0);
	sqe->user_data = ((unsigned long)(data) | IOURING_OP_TYPE_READ);
	/* io_uring_sqe_set_flags(sqe, IOSQE_BUFFER_SELECT); */
	sqe->flags = (__u8)(IOSQE_BUFFER_SELECT);
	sqe->buf_group = gid;
	return 0;
}

__attribute__((unused)) static int add_write_request(struct io_uring *ring,
						     int client_socket,
						     void *data, void *buf,
						     size_t len, uint8_t flags)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	io_uring_prep_send(sqe, client_socket, buf, len, 0);
	sqe->user_data = ((unsigned long)(data) | IOURING_OP_TYPE_WRITE);
	sqe->flags = (__u8)(flags);
	return 0;
}

__attribute__((unused)) static int
add_splice_request(struct io_uring *ring, int fd_in, int64_t off_in, int fd_out,
		   int64_t off_out, unsigned int len, uint8_t flags)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	io_uring_prep_splice(sqe, fd_in, off_in, fd_out, off_out, len, 0);
	sqe->user_data = ((unsigned long)(0) | IOURING_OP_TYPE_SPLICE);
	sqe->flags = (__u8)(flags);
	return 0;
}

__attribute__((unused)) static int add_buf_request(struct io_uring *ring,
						   void *addr, int len, int gid,
						   int bid, uint8_t flags)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
	io_uring_prep_provide_buffers(sqe, addr, len, 1, gid, bid);
	sqe->user_data = ((unsigned long)(0) | IOURING_OP_TYPE_PROVIDE_BUF);
	sqe->flags = (__u8)(flags);
	return 0;
}

#endif


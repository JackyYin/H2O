#ifndef _H2O_ROUTER_H
#define _H2O_ROUTER_H

#include "conn.h"

struct h2o_router {
	int root_fd;
	char *index_html;
	void (*handler)(void *, void *, void *, uint32_t, char *);
};

struct h2o_router *h2o_init_router(struct h2o_hashmap *map);

#endif


#ifndef _H2O_CONF_H
#define _H2O_CONF_H

struct h2o_config {
	int listen_on;
	char *config_file_path;
	void *config_file_addr_begin;
	void *config_file_addr_end;
};

struct h2o_config *h2o_get_default_config();

#endif


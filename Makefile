TEST_DIR ?= tests
SRC_DIR ?= src
INCLUDE_DIR ?= include
LIBURING_DIR ?= liburing
XXHASH_DIR ?= xxHash
GNUTLS_DIR ?= gnutls
MBEDTLS_DIR ?= mbedtls
LIB_DIR ?= $(SRC_DIR)/lib
TOOLS_DIR ?= $(SRC_DIR)/tools

LIB_SOURCE := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJECTS := $(patsubst %.c, %.o, $(LIB_SOURCE))

TEST_SOURCE := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_SOURCE))
UT_EXE = unittest

SRC_SOURCE := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJECTS := $(patsubst %.c, %.o, $(SRC_SOURCE))
SRC_EXE = h2o

MIMEGEN_EXE = mimegen

.PHONY: all runtests clean

all: gen-mime-header $(SRC_EXE)

runtests: $(TEST_DIR)/$(UT_EXE)
	$<

gen-mime-header: $(MIMEGEN_EXE)
	./$(MIMEGEN_EXE) ./mime.types > $(INCLUDE_DIR)/mime.h


$(TEST_DIR)/$(UT_EXE): $(TEST_OBJECTS) $(LIB_OBJECTS)
	$(CC) -o $@ $^ -lcunit -lxxhash -L$(XXHASH_DIR)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c $(LIBURING_DIR) $(MBEDTLS_DIR)
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -I$(LIBURING_DIR)/src/include -I$(MBEDTLS_DIR)/include -I$(MBEDTLS_DIR)/library -g -Wall -Wno-overflow $<

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(LIBURING_DIR) $(MBEDTLS_DIR)
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -I$(LIBURING_DIR)/src/include -I$(MBEDTLS_DIR)/include -I$(MBEDTLS_DIR)/library -g -Wall $<

$(LIB_DIR)/%.o: $(LIB_DIR)/%.c $(XXHASH_DIR)
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -I$(XXHASH_DIR) -g -Wall $<

$(SRC_EXE): $(SRC_OBJECTS) $(LIB_OBJECTS)
	$(CC) -o $@ $^ -luring -lxxhash -lmbedtls -lmbedcrypto -lmbedx509 -L$(LIBURING_DIR)/src -L$(XXHASH_DIR) -L$(MBEDTLS_DIR)/library

$(MIMEGEN_EXE): $(TOOLS_DIR)/mimegen.o $(LIB_OBJECTS)
	$(CC) -o $@ $^ -lxxhash -L$(XXHASH_DIR)

$(LIBURING_DIR):
	git clone https://github.com/axboe/liburing.git
	cd $(LIBURING_DIR) && ./configure
	make -C $(LIBURING_DIR)

$(XXHASH_DIR):
	git clone https://github.com/Cyan4973/xxHash.git
	make -C $(XXHASH_DIR)

$(MBEDTLS_DIR):
	wget --no-clobber https://github.com/Mbed-TLS/mbedtls/archive/refs/tags/mbedtls-2.28.5.tar.gz
	tar zxvf mbedtls-2.28.5.tar.gz
	mv mbedtls-mbedtls-2.28.5 $(MBEDTLS_DIR)
	CFLAGS="-g" make -C $(MBEDTLS_DIR) lib

clean:
	rm -rf $(LIBURING_DIR)
	rm -rf $(XXHASH_DIR)
	rm -rf $(MBEDTLS_DIR)
	rm -rf $(LIB_OBJECTS)
	rm -rf $(TOOLS_OBJECTS)
	rm -rf $(SRC_OBJECTS)
	rm -rf $(TEST_OBJECTS)
	rm -rf $(MIMEGEN_EXE) $(SRC_EXE) $(TEST_DIR)/$(UT_EXE)

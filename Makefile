TEST_DIR ?= tests
SRC_DIR ?= src
INCLUDE_DIR ?= include
LIBURING_DIR ?= liburing
XXHASH_DIR ?= xxHash
GNUTLS_DIR ?= gnutls
MBEDTLS_DIR ?= mbedtls

TEST_SOURCE := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_SOURCE))
UT_EXE = unittest

SRC_SOURCE := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJECTS := $(patsubst %.c, %.o, $(SRC_SOURCE))
SRC_EXE = h2o

.PHONY: all runtests clean

all: $(LIBURING_DIR) $(XXHASH_DIR) $(MBEDTLS_DIR) $(SRC_DIR)/$(SRC_EXE)

runtests: $(TEST_DIR)/$(UT_EXE)
	$<

$(TEST_DIR)/$(UT_EXE): $(TEST_OBJECTS) $(SRC_DIR)/huffman.o $(SRC_DIR)/hdt.o
	$(CC) -o $@ $^ -lcunit

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -g -Wall -Wno-overflow $<

$(SRC_DIR)/$(SRC_EXE): $(SRC_OBJECTS)
	$(CC) -o $@ $^ -luring -lxxhash -lmbedtls -lmbedcrypto -lmbedx509 -L$(LIBURING_DIR)/src -L$(XXHASH_DIR) -L$(MBEDTLS_DIR)/library

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -I$(LIBURING_DIR)/src/include -I$(XXHASH_DIR) -I$(MBEDTLS_DIR)/include -I$(MBEDTLS_DIR)/library -g -Wall $<

$(LIBURING_DIR):
	git clone https://github.com/axboe/liburing.git
	cd $(LIBURING_DIR) && ./configure
	make -C $(LIBURING_DIR)

$(XXHASH_DIR):
	git clone https://github.com/Cyan4973/xxHash.git
	make -C $(XXHASH_DIR)

$(WOLFSSL_DIR):
	git clone https://github.com/wolfSSL/wolfssl.git
	cd $(WOLFSSL_DIR) && ./configure
	make -C $(WOLFSSL_DIR)

$(MBEDTLS_DIR):
	# git clone https://github.com/Mbed-TLS/mbedtls.git
	wget https://github.com/Mbed-TLS/mbedtls/archive/refs/tags/mbedtls-2.28.5.tar.gz
	tar zxvf mbedtls-2.28.5.tar.gz
	mv mbedtls-mbedtls-2.28.5 $(MBEDTLS_DIR)
	CFLAGS="-g" make -C $(MBEDTLS_DIR) lib

clean:
	rm -rf $(LIBURING_DIR)
	rm -rf $(XXHASH_DIR)
	rm -rf $(MBEDTLS_DIR)
	rm -rf $(SRC_OBJECTS) $(SRC_DIR)/$(SRC_EXE)
	rm -rf $(TEST_OBJECTS) $(TEST_DIR)/$(UT_EXE)

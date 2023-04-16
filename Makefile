TEST_DIR ?= tests
SRC_DIR ?= src
INCLUDE_DIR ?= include
LIBURING_DIR ?= liburing

TEST_SOURCE := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_SOURCE))
UT_EXE = unittest

SRC_SOURCE := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJECTS := $(patsubst %.c, %.o, $(SRC_SOURCE))
SRC_EXE = h2o

.PHONY: all runtests clean

all: $(LIBURING_DIR) $(SRC_DIR)/$(SRC_EXE)

runtests: $(TEST_DIR)/$(UT_EXE)
	$<

$(TEST_DIR)/$(UT_EXE): $(TEST_OBJECTS) $(SRC_DIR)/huffman.o
	$(CC) -o $@ $^ -lcunit

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -g -Wall -Wno-overflow $<

$(SRC_DIR)/$(SRC_EXE): $(SRC_OBJECTS)
	$(CC) -o $@ $^ -luring -L$(LIBURING_DIR)/src

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -I$(LIBURING_DIR)/src/include -g -Wall $<

$(LIBURING_DIR):
	git clone https://github.com/axboe/liburing.git
	cd $(LIBURING_DIR) && ./configure
	make -C $(LIBURING_DIR)

clean:
	rm -rf $(LIBURING_DIR)
	rm -rf $(SRC_OBJECTS) $(SRC_DIR)/$(SRC_EXE)
	rm -rf $(TEST_OBJECTS) $(TEST_DIR)/$(UT_EXE)

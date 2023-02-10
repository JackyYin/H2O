TEST_DIR ?= tests
INCLUDE_DIR ?= include

TEST_SOURCE := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_SOURCE))
UT_EXE = unittest

.PHONY: tests clean

tests: $(TEST_DIR)/$(UT_EXE)
	$<

$(TEST_DIR)/$(UT_EXE): $(TEST_OBJECTS)
	$(CC) -o $@ $^ -lcunit

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c -o $@ -I$(INCLUDE_DIR) -g -Wall $<

clean:
	rm -rf $(TEST_OBJECTS) $(TEST_DIR)/$(UT_EXE)

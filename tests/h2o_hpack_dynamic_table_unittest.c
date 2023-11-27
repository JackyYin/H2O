#include "h2o_frame_header_unittest.h"
#include "hdt.h"
#include <errno.h>
#include <string.h>

static void test_HDT_peek_pop(void)
{
	hdt tbl;
	hdt_init(&tbl, 256);

	int ret;
	char key1[64] = "Custom-Header1";
	char value1[64] = "As you learnd from the previous chapter";
	hdt_push(&tbl, key1, strlen(key1), value1, strlen(value1));

	char k1[64] = {}, v1[64] = {};
	size_t k1len = sizeof(k1), v1len = sizeof(v1);
	ret = hdt_peek_nth(&tbl, k1, &k1len, v1, &v1len, 0);
	CU_ASSERT(0x00 == ret);
	CU_ASSERT_NSTRING_EQUAL(k1, key1, k1len);
	CU_ASSERT_NSTRING_EQUAL(v1, value1, v1len);

	hdt_pop(&tbl);

	char k2[64] = {}, v2[64] = {};
	size_t k2len = sizeof(k2), v2len = sizeof(v2);
	ret = hdt_peek_nth(&tbl, k2, &k2len, v2, &v2len, 0);
	CU_ASSERT(-ENOENT == ret);
}

bool h2o_hpack_dynamic_table_ut_register()
{
	CU_pSuite pSuite = CU_add_suite(
	    "H2 HPACK dynamic table", init_suite_general, clean_suite_general);

	if (!pSuite)
		return false;

	if (!CU_add_test(pSuite, "test pop element, then peek from HDT",
			 test_HDT_peek_pop))
		return false;

	return true;
}

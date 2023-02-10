#include "h2o.h"
#include "h2o_frame_header_unittest.h"

static void test_H2O_FRAME_HEADER_TYPE(void)
{
	struct h2o_frame_header h0 = {0, H2O_FRAME_TYPE_DATA, 0, 0, 0};
	struct h2o_frame_header h1 = {0, H2O_FRAME_TYPE_HEADER, 0, 0, 0};
	struct h2o_frame_header h2 = {0, H2O_FRAME_TYPE_PRIORITY, 0, 0, 0};
	struct h2o_frame_header h3 = {0, H2O_FRAME_TYPE_RST_STREAM, 0, 0, 0};
	struct h2o_frame_header h4 = {0, H2O_FRAME_TYPE_SETTINGS, 0, 0, 0};
	struct h2o_frame_header h5 = {0, H2O_FRAME_TYPE_PUSH_PROMISE, 0, 0, 0};
	struct h2o_frame_header h6 = {0, H2O_FRAME_TYPE_PING, 0, 0, 0};
	struct h2o_frame_header h7 = {0, H2O_FRAME_TYPE_GOAWAY, 0, 0, 0};
	struct h2o_frame_header h8 = {0, H2O_FRAME_TYPE_WINDOW_UPDATE, 0, 0, 0};
	struct h2o_frame_header h9 = {0, H2O_FRAME_TYPE_CONTINUATION, 0, 0, 0};

	CU_ASSERT(0x00 == h0.type);
	CU_ASSERT(0x01 == h1.type);
	CU_ASSERT(0x02 == h2.type);
	CU_ASSERT(0x03 == h3.type);
	CU_ASSERT(0x04 == h4.type);
	CU_ASSERT(0x05 == h5.type);
	CU_ASSERT(0x06 == h6.type);
	CU_ASSERT(0x07 == h7.type);
	CU_ASSERT(0x08 == h8.type);
	CU_ASSERT(0x09 == h9.type);
}

static void test_H2O_FRAME_HEADER_RESERVED(void)
{
	struct h2o_frame_header h0 = {0, H2O_FRAME_TYPE_DATA, 0xff, 0, 0xff};
	CU_ASSERT(0 == h0.reserved);

	struct h2o_frame_header h1 = {0, H2O_FRAME_TYPE_DATA, 0xff, 1, 0xff};
	CU_ASSERT(1 == h1.reserved);
}

bool h2o_frame_header_ut_register()
{
	CU_pSuite pSuite = CU_add_suite("H2 FRAME HEADER", init_suite_general,
					clean_suite_general);

	if (!pSuite)
		return false;

	if (!CU_add_test(pSuite, "test of frame header types",
			 test_H2O_FRAME_HEADER_TYPE))
		return false;

	if (!CU_add_test(pSuite, "test of frame header reserved bit field",
			 test_H2O_FRAME_HEADER_RESERVED))
		return false;

	return true;
}

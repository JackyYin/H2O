#include <CUnit/Basic.h>
#include <stdint.h>

#include "h2o_frame_header_unittest.h"
#include "h2o_hpack_dynamic_table_unittest.h"
#include "h2o_huffman_decoder_unittest.h"

int init_suite_general(void) { return 0; }
int clean_suite_general(void) { return 0; }

bool (*register_funcs[])(void) = {h2o_frame_header_ut_register,
				  h2o_huffman_decoder_ut_register,
				  h2o_hpack_dynamic_table_ut_register};

int main()
{
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	for (int i = 0; i < (sizeof(register_funcs) / sizeof(uintptr_t)); i++) {
		if (!register_funcs[i]()) {
			CU_cleanup_registry();
			return CU_get_error();
		}
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

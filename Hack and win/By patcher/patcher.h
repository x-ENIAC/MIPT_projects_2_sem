#include "work_with_file.h"

const long long need_hash_file = 40000006184;

enum CODE_ASM_OPERATIONS {
	NOP	= 0x90,
	RET	= 0xc3,
};

ERROR_CODE check_hash_kill_file(struct file_info* file_to_kill);
ERROR_CODE begin_do_evil(struct file_info* file_to_kill);

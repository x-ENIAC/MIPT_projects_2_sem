#include <stdio.h>

enum ERROR_CODE {
	ALL_IS_OKEY		= 0,
	NO_FILE_NAME	= 1,
	FILE_NOT_EXIST	= 2,
	ERROR_READ_FILE	= 3,
	BAD_HASH_FILE	= 4
};

struct file_info {
	FILE* file_ptr;
	char* buffer;
	int size_buffer;
	ERROR_CODE status;
};

ERROR_CODE read_buffer(struct file_info* file_to_kill, int additional_buffer_size);
int size_of_buffer(FILE* file);

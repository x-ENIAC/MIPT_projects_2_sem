#include <stdio.h>
#include <stdlib.h>
#include "patcher.h"
#include "get_hash.h"

long long get_simple_hash(struct file_info* file_to_kill) {
	if(!file_to_kill)
		return 0;

	long long hash = 0;
	for(int i = 0; i < file_to_kill->size_buffer; ++i)
		hash = (hash + file_to_kill->buffer[i] * first_big_number) % second_big_number;

	return hash;
}
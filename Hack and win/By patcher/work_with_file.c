#include <stdio.h>
#include <stdlib.h>
#include "work_with_file.h"

ERROR_CODE read_buffer(struct file_info* file_to_kill, int additional_buffer_size) {
	file_to_kill->size_buffer = size_of_buffer(file_to_kill->file_ptr);
	file_to_kill->buffer = (char*)calloc(file_to_kill->size_buffer + additional_buffer_size + 5, sizeof(char));
	int status = fread(file_to_kill->buffer, sizeof(char), file_to_kill->size_buffer, file_to_kill->file_ptr);

	if(status != file_to_kill->size_buffer) {
		printf("Error in read file\n");
		file_to_kill->status = ERROR_READ_FILE;
		return ERROR_READ_FILE;
	}	

	return ALL_IS_OKEY;
}

int size_of_buffer(FILE* file) { 
    int length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}
#include <stdio.h>
#include <stdlib.h>
#include "patcher.h"
#include "get_hash.h"

int main() {	
	// ------------------ Read file with my jokes -------------------
	struct file_info happiness_file = {};
	if((happiness_file.file_ptr = fopen("sun.com", "rb")) == NULL) {
		printf("Sorry, input file doesn't exist\n");
		return FILE_NOT_EXIST;
	}	

	ERROR_CODE status = read_buffer(&happiness_file, 0);	
	if(status != ALL_IS_OKEY) {
		printf("Error in read from file\n");
		fclose(happiness_file.file_ptr);
		return status;
	}	

	// ------------------ Read file to kill -------------------------
	struct file_info file_to_kill = {}; 

	if((file_to_kill.file_ptr = fopen("anna.com", "rb")) == NULL) {
		printf("Sorry, input file doesn't exist\n");
		return FILE_NOT_EXIST;
	}

	status = read_buffer(&file_to_kill, file_to_kill.size_buffer + 1);	

	if(status != ALL_IS_OKEY) {
		printf("Error in read from file\n");
		return status;
	}


	status = check_hash_kill_file(&file_to_kill);
	if(status != ALL_IS_OKEY) {
		printf("Bad hash file\n");
		return status;
	}

	begin_do_evil(&file_to_kill);

	fclose(happiness_file.file_ptr);

	// ------------------ Write killed file ------------------------

	FILE* killed_file = {};
	if((killed_file = fopen("killed.com", "wb")) == NULL) {
		printf("Sorry, output file doesn't open\n");
		fclose(killed_file);
		return FILE_NOT_EXIST;		
	}

	fwrite(file_to_kill.buffer, sizeof(char), 129 + 77 + 50, killed_file);	
	fwrite(happiness_file.buffer, sizeof(char), happiness_file.size_buffer, killed_file);

	fclose(killed_file);
		
		

	printf("The creation of evil was successful, he-he\n");

	return ALL_IS_OKEY;
}

ERROR_CODE check_hash_kill_file(struct file_info* file_to_kill) {
	long long now_hash = get_simple_hash(file_to_kill);
	printf("%lld - hash your file\n", now_hash);

	if(now_hash != need_hash_file)
		return BAD_HASH_FILE;

	return ALL_IS_OKEY;
}

ERROR_CODE begin_do_evil(struct file_info* file_to_kill) {
	for(int i = 22; i <= 29; ++i)
		file_to_kill->buffer[i] = NOP; 

	for(int i = 109; i < 129; ++i)
		file_to_kill->buffer[i] = NOP; 
	file_to_kill->buffer[129] = RET;

	char* copy_function = (char*)calloc(75, sizeof(char));
	for(int i = 130; i < 134; ++i)
		file_to_kill->buffer[i] = NOP;
	
	for(int i = 134; i < 134 + 123; ++i) {
		copy_function[i - 134] = file_to_kill->buffer[i];
	}

	return ALL_IS_OKEY;
}
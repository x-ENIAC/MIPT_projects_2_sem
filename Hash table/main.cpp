#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>
#include <time.h>
#include "warnings.h"

#define VERIFY_FILE_STATUS                                                  \
    if(file_status != FILE_OK) {                                            \
        return file_status;                                                 \
    }

#define VERIFY_HASH_TABLE_STATUS                                                  		  \
    if(hash_table_status != HASH_TABLE_OKEY) {                                            \
        return hash_table_status;             		                                      \
    }

int main() {

	srand(time(NULL));

 	File file_with_dict = file_construct("dict.txt");

 	FILE_STATUS file_status = read_buffer(&file_with_dict);
 	VERIFY_FILE_STATUS

	Hash_table_type hash_table = {};
	HASH_TABLE_STATUSES hash_table_status = hash_table_construct(&hash_table);
	VERIFY_HASH_TABLE_STATUS

 	FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);	

	printf("begin parse buffer\n");
 	hash_table_status = parsing_buffer(&file_with_dict, &hash_table);

	clock_t begin_time = clock();
 	testing_hash_table(&hash_table);
	clock_t end_time = clock();

	printf("\n%lu ms\n", (end_time - begin_time) / 1000);

	char* test = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
	memcpy(test, "poison", strlen("poison") * sizeof(char));
	uint32_t hash = get_hash_word(test);
	print_list(&hash_table.chains[hash]);

	hash_table_destruct(&hash_table);
	file_destruct(&file_with_dict);

	return 0;
}
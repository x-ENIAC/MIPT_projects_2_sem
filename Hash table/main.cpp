#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>
#include <time.h>
#include "warnings.h"

#define VERIFY_FILE_STATUS                                                  \
    if(file_status != FILE_OK) {                                            \
        /*warning(TEXT_FILE_STATUS[file_status], INFORMATION_ABOUT_CALL);*/     \
        return file_status;                                                 \
    }

#define VERIFY_HASH_TABLE_STATUS                                                  		  \
    if(hash_table_status != HASH_TABLE_OKEY) {                                            \
        /*warning(TEXT_HASH_TABLE_STATUSES[hash_table_status], INFORMATION_ABOUT_CALL);*/     \
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

	/*int count = 0;
	for(int i = 0; i < SIZE_HASH_TABLE; ++i)
		if(hash_table.chains[i].size_list != 0)
			++count;

	printf("not 0: %d\n", count);*/

	//long long hash = get_hash_word("poison", strlen("poison"));
	//print_list(&hash_table.chains[hash]);

	hash_table_destruct(&hash_table);
	file_destruct(&file_with_dict);

	return 0;
}
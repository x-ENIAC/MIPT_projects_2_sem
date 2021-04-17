#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>
#include <time.h>
#include "warnings.h"

#define VERIFY_FILE_STATUS                                                  \
    if(file_status != FILE_OK) {                                            \
        warning(TEXT_FILE_STATUS[file_status], INFORMATION_ABOUT_CALL);     \
        return FILE_BAD_PTR;                                                \
    }

int main() {

	srand(time(NULL));

	clock_t begin_time = clock();

	srand( time( 0 ) );	

 	File file_with_dict = {};
 	file_with_dict.name_file = "dict.txt";

 	FILE_STATUS file_status = read_buffer(&file_with_dict);
 	VERIFY_FILE_STATUS

	Hash_table_type hash_table = {};
	HASH_TABLE_STATUSES hash_table_status = hash_table_construct(&hash_table);

 	FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);	

	printf("begin parse buffer\n");
 	hash_table_status = parsing_buffer(&file_with_dict, &hash_table);

 	testing_hash_table(&hash_table);

	clock_t end_time = clock();

	printf("\n%lu ms\n", (end_time - begin_time) / 1000);

	//print_something_chain(&hash_table, 48850);
	long long hash = get_hash_word("cat", strlen("cat"));
	print_list(&hash_table.chains[hash]);

	 hash_table_destruct(&hash_table);

	return 0;
}
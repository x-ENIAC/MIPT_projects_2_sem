#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>
#include <time.h>

int main() {

	srand(time(NULL));

	clock_t begin_time = clock();

	srand( time( 0 ) );	

 	File file_with_dict = {};
 	file_with_dict.name_file = "dict.txt";

 	FILE_STATUS file_status = read_buffer(&file_with_dict);

	Hash_table_type hash_table = {};
	HASH_TABLE_STATUSES hash_table_status = hash_table_construct(&hash_table);

 	FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);	

	printf("begin parse buffer\n");
 	hash_table_status = parsing_buffer(&file_with_dict, &hash_table);

 	testing_hash_table(&hash_table);

 	hash_table_destruct(&hash_table);

	clock_t end_time = clock();

	printf("\n%lu ms\n", (end_time - begin_time) / 1000);

	return 0;
}
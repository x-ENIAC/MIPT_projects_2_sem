#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main() {
 	File file_with_dict = {};
 	file_with_dict.name_file = "dict.txt";

 	FILE_STATUS file_status = read_buffer(&file_with_dict);

 	printf("%d\n%s\n", file_with_dict.size_buffer, TEXT_FILE_STATUS[file_status]);

	Hash_table_type hash_table = {};
	HASH_TABLE_STATUSES hash_table_status = hash_table_construct(&hash_table);
	printf("%s\n", TEXT_HASH_TABLE_STATUSES[hash_table_status]);

 	FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);	

 	hash_table_status = parsing_buffer(&file_with_dict, &hash_table);
 	printf("%s\n", TEXT_HASH_TABLE_STATUSES[hash_table_status]);

 	printf("\ndone\n");

	//print_list(&hash_table.chains[6]);
	print_list(&hash_table.chains[8]);	// lucky: udacha
	//print_list(&hash_table.chains[14]);
 	print_list(&hash_table.chains[28]);	// hello: privet
 	print_list(&hash_table.chains[34]); // happy: schaste
 	//print_list(&hash_table.chains[68]);

	return 0;
}
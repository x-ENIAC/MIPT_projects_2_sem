#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>

int main() {
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

	//print_list(&hash_table.chains[6]);
	/*for(int number_list = 0; number_list < SIZE_HASH_TABLE; ++number_list) {
		printf("%d list:\n", number_list);
		print_list(&hash_table.chains[number_list]);	// lucky: udacha
		printf("\n\n\n");
	}*/

 	//char* h = "allocate";
 	//printf("%s - %lld\n", "allocate", get_hash_word(h, 8));
 	printf("%s - %lld\n", "dish", get_hash_word("dish", 4));
 	printf("%s - %lld\n", "heir", get_hash_word("heir", 4));
 	printf("%s - %lld\n", "mind", get_hash_word("mind", 4));

 	char* checked_word = "mind"; //(char*)calloc(100, sizeof(char));
 	//checked_word[0] = 'y'; checked_word[1] = 'e'; checked_word[2] = 'a'; checked_word[3] = 'r';
 	bool result = hash_table_contain_element(&hash_table, checked_word, 4);

 	if(result)
 		printf("YES\n");
 	else
 		printf("NO\n");

	return 0;
}
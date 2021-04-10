#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
//#include "list.h"

#define CHECK_HASH_TABLE_STATUS								\
	if(status != HASH_TABLE_OKEY) {							\
		printf("%s\n", TEXT_HASH_TABLE_STATUSES[status]);	\
	}

HASH_TABLE_STATUSES hash_table_construct(Hash_table_type* hash_table) {
	if(!hash_table)
		return HASH_TABLE_BAD_POINTER;

	hash_table->size_table = SIZE_HASH_TABLE;
	hash_table->chains = (List*)calloc(hash_table->size_table, sizeof(List));

	if(!hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	for(int pos = 0; pos < hash_table->size_table; ++pos)
    	list_construct(&hash_table->chains[pos]);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES parsing_buffer(File* file_with_dict, Hash_table_type* hash_table) {
	if(!file_with_dict || !file_with_dict->buffer || !hash_table) // || !hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	char* begin_word = file_with_dict->buffer, *ptr_to_key = file_with_dict->buffer;
	int length_word = 0;
	bool find_key = false;
	long long hash_key = 0;

	if(file_with_dict->buffer[0] != ':')
		++length_word;

	long long hash_word = 0;

	for(int pos = 0; pos < file_with_dict->size_buffer; ++pos) {
		/*if(file_with_dict->buffer[pos] == ':' || file_with_dict->buffer[pos] == '\n') {		// find key
			--length_word;

			if(file_with_dict-<buffer[pos] == ':')
				find_key = true;

			hash_word = get_hash_word(begin_word, length_word);
			begin_word = file_with_dict->buffer + pos + 1;

			if(hash_word == BAD_HASH) {
				printf("bad hash, pos = %d\n", pos);
				continue;
			} else {
				printf("hash: %lld\n", hash_word);
			}

			if(file_with_dict->buffer[pos] == ':')
			else
				list_insert_before(hash_table->chains[hash_key], 1, );

			length_word = 1;
		}
		else 
			++length_word;*/

		printf("now symbol: %c\n", file_with_dict->buffer[pos]);
		if(file_with_dict->buffer[pos] == ':') {
			--length_word;

			ptr_to_key = begin_word;
			hash_word = get_hash_word(begin_word, length_word);
			begin_word = file_with_dict->buffer + pos + 1;

			if(hash_word == BAD_HASH) {
				printf("bad hash, pos = %d\n", pos);
				continue;
			} else {
				printf("hash: %lld\n", hash_word);
			}
		
			hash_key = hash_word;

			length_word = 1;

		} else if(file_with_dict->buffer[pos] == '\n') {
			--length_word;

			printf("well, I find value!\n");
			for(int j = 0; j < length_word; ++j)
				printf("%c", *(begin_word + j));
			printf("\nokey\n");
			list_insert_before(&hash_table->chains[hash_key], 1, begin_word, length_word);

			begin_word = file_with_dict->buffer + pos + 1;	
			length_word = 1;

		} else
			++length_word;
	}

	return HASH_TABLE_OKEY;
}

long long get_hash_word(const char* word, const int length_word) {
	if(!word || length_word < 0)
		return BAD_HASH;

	long long hash = 0;

	for(int pos = 0; pos < length_word; ++pos) {
		hash = (hash + (word[pos] * FIRST_BIG_NUMBER) % SIZE_HASH_TABLE ) % SIZE_HASH_TABLE;
	}

	return hash;
}
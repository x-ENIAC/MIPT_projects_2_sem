#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <nmmintrin.h>

extern "C" unsigned int asm_get_hash(const unsigned int, const unsigned int, const unsigned int);		

#define FIRST_ACCELERATION
#define SECOND_ACCELERATION

#define CHECK_HASH_TABLE_STATUS								\
	if(status != HASH_TABLE_OKEY) {							\
		printf("%s\n", TEXT_HASH_TABLE_STATUSES[status]);	\
	}

HASH_TABLE_STATUSES hash_table_construct(Hash_table_type* hash_table) {
	if(!hash_table)
		return HASH_TABLE_BAD_POINTER;

	hash_table->size_table = SIZE_HASH_TABLE;
	hash_table->chains = (List*)calloc(hash_table->size_table + 5, sizeof(List));

	if(!hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	for(unsigned int pos = 0; pos < hash_table->size_table; ++pos)
	   	list_construct(hash_table->chains + pos);
	
	return HASH_TABLE_OKEY;
}

//int counter = 0;

HASH_TABLE_STATUSES parsing_buffer(File* file_with_dict, Hash_table_type* hash_table) {
	if(!file_with_dict || !file_with_dict->buffer || !hash_table || !hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	char* begin_word = file_with_dict->buffer, *ptr_to_key = file_with_dict->buffer;
	unsigned int length_word = 0, length_key = 0;
	bool find_key = false;

	if(file_with_dict->buffer[0] != ':')
		++length_word;

	for(unsigned int pos = 0; pos < file_with_dict->size_buffer; ++pos) {
		if(file_with_dict->buffer[pos] == ' ' && !find_key)
			continue;

		if(file_with_dict->buffer[pos] == ':') {
			--length_word;

			ptr_to_key  = begin_word;
			length_key  = length_word;
			begin_word  = file_with_dict->buffer + pos + 2;

			find_key 	= true;

			length_word = 0;

		} else if(file_with_dict->buffer[pos] == '\n') {
			--length_word;

			hash_table_insert_element_by_key(hash_table, ptr_to_key, length_key, begin_word, length_word);

			begin_word  = file_with_dict->buffer + pos + 1;	
			length_word = 1;

			find_key = false;
			//++counter;

		} else
			++length_word;
	}

	//printf("============count: %d\n", counter);

	return HASH_TABLE_OKEY;
}

unsigned int get_hash_word(const char* word, const unsigned int length_word) {
	if(!word)
		return BAD_HASH;

	unsigned int hash = 0;

	#ifndef FIRST_ACCELERATION

		/*size_t delta = 1;

		for(size_t pos = 0; pos < length_word; ++pos) {
			hash = (hash + (((CONSTANT_TO_HASHING + word[pos] - 'a') % CONSTANT_TO_HASHING + 1) * delta) % SIZE_HASH_TABLE) % SIZE_HASH_TABLE;
			delta += DELTA_FOR_DELTA;
		}*/

		#define POLYNOM 0x82f63b78
		//int length = strlen(word);
		//printf("%s: %d\n", word, length);


		unsigned int iterations_amount = MAX_SIZE_VALUE / 4;
		for (int i = 0; i < iterations_amount; i++) {
			unsigned index = i * 4;
			unsigned number = (((((word[index] << iterations_amount) + word[index + 1]) << iterations_amount) + word[index + 2]) << iterations_amount) + word[index + 3];
			hash += _mm_crc32_u32(POLYNOM, number);
		}

		hash %= SIZE_HASH_TABLE;

		/*unsigned int crc = 0;
		crc = ~crc;
		for(int i = 0; i < length_word; ++i) {
			crc ^= word[i];
			for(int k = 0; k < 8; k++)
				crc = crc & 1 ? (crc >> 1) ^ POLY2 : crc >> 1;
		}*/
		
		//hash = crc;	

		/*if(counter % 1000 == 0)
					printf("!!!!!!!!!!!!!! %d\n", counter);
		++counter;*/

	#else

		//hash = asm_get_hash(word, length_word, SIZE_HASH_TABLE, DELTA_FOR_DELTA);
		hash = asm_get_hash(0, *(unsigned int*)word, 13);

	#endif

	return hash;
}

bool hash_table_is_contain_element(Hash_table_type* hash_table, const char* value, const unsigned int length, bool is_print_values) {
	if(!hash_table || !value || length <= 0) 
		return false;

	unsigned int hash_word = get_hash_word(value, length);

	if(hash_table->chains[hash_word].size_list <= 0)
		return false;

	int is_find = 0;
	unsigned int now_pos = hash_table->chains[hash_word].head;
	//printf("%lu, hash: %llu\n", (counter++), hash_word);

	for(unsigned int word = 0; word < hash_table->chains[hash_word].size_list && now_pos != hash_table->chains[hash_word].tail; ++word) {
		#ifndef SECOND_ACCELERATION
			//printf("!!!first!!! %s (%lu)\n",  value, length);
			//printf("!!!second!!! %s (%lu)\n", hash_table->chains[hash_word].data[now_pos].value, hash_table->chains[hash_word].data[now_pos].length_value);
			//printf("\t%d\n", strcmp(value, hash_table->chains[hash_word].data[now_pos].key));
			if(!strcmp(value, hash_table->chains[hash_word].data[now_pos].key)) {
				is_find = true;
				break;
			}

		#else
			//printf("\t\tcounter: %d\n", (counter++));
			int is_equals_first_half = 0, is_equals_second_half = 0;

			asm ("vmovdqu ymm0, [%[first_string]]					\n "
			     "vmovdqu ymm1, [%[second_string]]					\n "
			     "vpcmpeqq ymm2, ymm0, ymm1							\n " // 0, if equal
			     "vpmovmskb %[is_equals_first_half], ymm2			\n " 

			     "vmovdqu ymm0, [[%[first_string]]  + 32]			\n "
			     "vmovdqu ymm1, [[%[second_string]] + 32]			\n "
			     "vpcmpeqq ymm2, ymm0, ymm1							\n " // 0, if equal
			     "vpmovmskb %[is_equals_second_half], ymm2			\n "

			     "not %[is_equals_first_half]						\n "
			     "not %[is_equals_second_half]						\n "
				
			    : [is_equals_first_half]"=&r" (is_equals_first_half), [is_equals_second_half]"=&r"(is_equals_second_half)	
			    : [first_string]"r" (value), [second_string]"r"(hash_table->chains[hash_word].data[now_pos].value)
			    : "ymm0", "ymm1", "ymm2", "cc"
			);

			if(!is_equals_first_half) {
				is_find = true;
				break;
			}

		#endif

		now_pos = hash_table->chains[hash_word].data[now_pos].next;
	}

	if(is_find && is_print_values)
		print_list(&hash_table->chains[hash_word]);

	return is_find;
}

HASH_TABLE_STATUSES hash_table_insert_element_by_key(Hash_table_type* hash_table, const char* key,   const unsigned int length_key, 
																				  const char* value, const unsigned int length_value) {
	if(!hash_table || !hash_table->chains || !key || !value)
		return HASH_TABLE_BAD_POINTER; 

	if(length_key <= 0 || length_value <= 0)
		return HASH_TABLE_BAD_SIZE;

	unsigned int hash_key = get_hash_word(key, length_key);
	list_insert_before(hash_table->chains + hash_key, hash_table->chains[hash_key].size_list + 1, key, length_key, value, length_value);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES hash_table_destruct(Hash_table_type* hash_table) {
	if(!hash_table)
		return HASH_TABLE_OKEY;

	for(unsigned int list_number = 0; list_number < hash_table->size_table; ++list_number)
		list_destruct(&hash_table->chains[list_number]);

	free(hash_table->chains);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES testing_hash_table(Hash_table_type* hash_table) {
	if(!hash_table) 
		return HASH_TABLE_BAD_POINTER;


	for(unsigned int test = 0; test < COUNT_OF_TESTS; ++test) {
		//printf("test %lu\n", test);
		unsigned int length_word = rand() % (MAX_SIZE_KEY - 2) + 1;
		char word[length_word + 1] = "";

		for(unsigned int pos = 0; pos < length_word; ++pos) {
			word[pos] = get_random_symbol();
		}
		
		//printf("\n\nLENGTH\t\t%lu, STRING %s\n", length_word, word);
		//printf("-----------------------------------\n");
		hash_table_is_contain_element(hash_table, word, length_word);

	}

	return HASH_TABLE_OKEY;
}

inline char get_random_symbol() {
	return rand() % ('z' - 'a' + 1) + 'a';
}

void print_something_chain(Hash_table_type* hash_table, const size_t hash) {
	for(unsigned int i = 0; i < hash_table->chains[hash].size_list; ++i)
		printf("%s\n", hash_table->chains[hash].data[i].key);
}
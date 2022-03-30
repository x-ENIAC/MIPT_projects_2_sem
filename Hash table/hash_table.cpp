#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <nmmintrin.h>

extern "C" uint32_t asm_get_hash(const uint32_t, const uint32_t, const uint32_t);		

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

	for(uint32_t pos = 0; pos < hash_table->size_table; ++pos)
	   	list_construct(hash_table->chains + pos);
	
	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES parsing_buffer(File* file_with_dict, Hash_table_type* hash_table) {
	if(!file_with_dict || !file_with_dict->buffer || !hash_table || !hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	char* begin_word = file_with_dict->buffer, *ptr_to_key = file_with_dict->buffer;
	uint32_t length_word = 0, length_key = 0;
	bool find_key = false;

	if(file_with_dict->buffer[0] != ':')
		++length_word;

	for(uint32_t pos = 0; pos < file_with_dict->size_buffer; ++pos) {
		if(file_with_dict->buffer[pos] == ' ' && !find_key)
			continue;

		if(file_with_dict->buffer[pos] == ':') {
			--length_word;

			/*if(length_word >= 64) {
				char* ptr = begin_word;
				for(int i = 0; i < length_word; ++i)
					printf("%c", ptr[i]);
				printf("\n!!!!\n");
			}*/		

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

		} else
			++length_word;
	}

	return HASH_TABLE_OKEY;
}

uint32_t get_hash_word(const char* word) {
	if(!word)
		return BAD_HASH;

	uint32_t hash = 0;

	#ifndef FIRST_ACCELERATION

		uint32_t iterations_amount = MAX_SIZE_KEY / 4;
		for (uint32_t iter = 0; iter < iterations_amount; ++iter) {
			uint32_t index = iter * 4;
			unsigned number = (((((word[index]      << SIZEOF_CHAR) 
				   				 + word[index + 1]) << SIZEOF_CHAR) 
								 + word[index + 2]) << SIZEOF_CHAR) 
								 + word[index + 3];

			hash += _mm_crc32_u32(POLYNOM_FOR_CRC32, number);
		}

		hash %= SIZE_HASH_TABLE;


	#else

		hash = asm_get_hash(0, *(uint32_t*)word, POWER_TWO_THAN_EQUAL_SIZE_HASH_TABLE);

	#endif

	return hash;
}

bool hash_table_is_contain_element(Hash_table_type* hash_table, const char* value, const uint32_t length, bool is_print_values) {
	if(!hash_table || !value || length <= 0) 
		return false;

	uint32_t hash_word = get_hash_word(value);

	if(hash_table->chains[hash_word].size_list <= 0)
		return false;

	int is_find = 0;

	#ifndef SECOND_ACCELERATION

	for(uint32_t word = 0; word < hash_table->chains[hash_word].size_list; ++word) {
		if(!strcmp(value, hash_table->chains[hash_word].data[word].key)) {
			is_find = true;
			break;
		}
	}

	#else
		int is_equals_first_half = 0, is_equals_second_half = 0, node_size = sizeof(Node);

		asm ("mov rcx, 0										\n " // now_pos_into_word
			 "mov rsi, %[size_list]								\n " // size_list
			 "mov rdi, %[ptr_to_list_value]						\n " // ptr_to_list_value

		  "finding_word:										\n "
		  	 "cmp rcx, rsi										\n "
		  	 "je end_of_compare									\n "

			 "vmovdqu ymm0, [%[string_to_cmp]]					\n " // the value we are looking for		     
			 "vmovdqu ymm1, [rdi + rcx * %[node_size]]			\n "		
		     "inc rcx											\n "

			 "vpcmpeqq ymm2, ymm0, ymm1							\n " // 0, if equal
			 "vpmovmskb %[is_equals_first_half], ymm2			\n " 

			 "vmovdqu ymm0, [%[string_to_cmp]         + 32]		\n "
			 "vmovdqu ymm1, [rdi + rcx * %[node_size] + 32]		\n "
			 "vpcmpeqq ymm2, ymm0, ymm1							\n " // 0, if equal
			 "vpmovmskb %[is_equals_second_half], ymm2			\n "

			 "not %[is_equals_first_half]						\n "
			 "not %[is_equals_second_half]						\n "

			 "cmp %[is_equals_first_half],  0					\n"
			 "jne finding_word									\n "

			 "cmp %[is_equals_second_half], 0					\n "
			 "jne finding_word									\n "

		     "mov %[is_find], 1									\n "
		     "jmp end_of_compare								\n "


		  "end_of_compare:										\n "
				
			: [is_equals_first_half]"=&r" (is_equals_first_half), [is_equals_second_half]"=&r"(is_equals_second_half)	
			: [string_to_cmp]"r" (value), [ptr_to_list_value]"r"(hash_table->chains[hash_word].data), 
			  [node_size]"r"(node_size), [size_list]"r"(hash_table->chains[hash_word].size_list), [is_find]"r"(is_find)
			: "ymm0", "ymm1", "ymm2", "rcx", "rsi", "rdi", "cc"
		);

	#endif

	if(is_find && is_print_values)
		print_list(&hash_table->chains[hash_word]);

	return is_find;
}

HASH_TABLE_STATUSES hash_table_insert_element_by_key(Hash_table_type* hash_table, const char* key,   const uint32_t length_key, 
																				  const char* value, const uint32_t length_value) {
	if(!hash_table || !hash_table->chains || !key || !value)
		return HASH_TABLE_BAD_POINTER; 

	if(length_key <= 0 || length_value <= 0)
		return HASH_TABLE_BAD_SIZE;


	uint32_t shortened_length_key = length_key;
	if(length_key >= MAX_SIZE_KEY)
		shortened_length_key = MAX_SIZE_KEY;

	char* shortened_key = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
	memcpy(shortened_key, key, shortened_length_key * sizeof(char));
	uint32_t hash_key = get_hash_word(shortened_key);

	list_insert_before(hash_table->chains + hash_key, hash_table->chains[hash_key].size_list + 1, key, shortened_length_key, value, length_value);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES hash_table_destruct(Hash_table_type* hash_table) {
	if(!hash_table)
		return HASH_TABLE_OKEY;

	for(uint32_t list_number = 0; list_number < hash_table->size_table; ++list_number)
		list_destruct(&hash_table->chains[list_number]);

	free(hash_table->chains);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES testing_hash_table(Hash_table_type* hash_table) {
	if(!hash_table) 
		return HASH_TABLE_BAD_POINTER;


	for(uint32_t test = 0; test < COUNT_OF_TESTS; ++test) {
		uint32_t length_word = rand() % (MAX_SIZE_KEY - 2) + 1;

		char word[length_word + 1] = "";

		for(uint32_t pos = 0; pos < length_word; ++pos) {
			word[pos] = get_random_symbol();
		}
		
		hash_table_is_contain_element(hash_table, word, length_word);

	}

	return HASH_TABLE_OKEY;
}

inline char get_random_symbol() {
	return rand() % ('z' - 'a' + 1) + 'a';
}

void print_something_chain(Hash_table_type* hash_table, const size_t hash) {
	for(uint32_t i = 0; i < hash_table->chains[hash].size_list; ++i)
		printf("%s\n", hash_table->chains[hash].data[i].key);
}
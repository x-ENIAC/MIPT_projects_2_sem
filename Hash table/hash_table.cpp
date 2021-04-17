#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

extern "C" int asm_get_hash(const char*, const long long, const long long, const long long);		

#define FIRST_ACCELERATION

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
	if(!file_with_dict || !file_with_dict->buffer || !hash_table || !hash_table->chains)
		return HASH_TABLE_BAD_POINTER;

	char* begin_word = file_with_dict->buffer, *ptr_to_key = file_with_dict->buffer;
	int length_word = 0, length_key = 0;
	bool find_key = false;

	if(file_with_dict->buffer[0] != ':')
		++length_word;

	long long hash_word = 0;

	for(int pos = 0; pos < file_with_dict->size_buffer; ++pos) {
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
			length_word -= 1;

			hash_table_insert_element_by_key(hash_table, ptr_to_key, length_key, begin_word, length_word);

			begin_word  = file_with_dict->buffer + pos + 1;	
			length_word = 1;

			find_key = false;

		} else
			++length_word;
	}

	return HASH_TABLE_OKEY;
}

unsigned long long get_hash_word(const char* word, const long long length_word) {
	if(!word || length_word < 0)
		return BAD_HASH;

	unsigned long long hash = 0, delta = 1;

	#ifndef FIRST_ACCELERATION

		//printf("Not asm\n");
		for(long long pos = 0; pos < length_word; ++pos) {
			hash = (hash + (((256 + word[pos] - 'a') % 256 + 1) * delta) % SIZE_HASH_TABLE) % SIZE_HASH_TABLE;
			delta += DELTA_FOR_DELTA;
		printf("%c - %lld (%d, %d)\n", word[pos], hash, word[pos], ((256 + word[pos] - 'a') % 256 + 1));

		}

	#else
		//printf("Assembler insertion\n");
		/*asm (
			 "mov r12, %1		\n " // r12 = my_string
			 "xor r10, r10		\n " // r10 = hash
			 "mov rbx, %2		\n "	// rbx = delta
			 "mov rcx, %3		\n " // rcx = length
			 "mov r15, %4		\n " // r15 = SIZE_HASH_TABLE
			 "mov r14, %5		\n " // r14 = DELTA_FOR_DELTA
			 "xor r11, r11		\n "

		  "get_hash:			\n "
			 "cmp rcx, 0		\n "
			 "je end_get_hash	\n "
			 "mov r11b, [r12]	\n "
			 "sub r11b, 96		\n "
			 "mov rax, r11		\n "
			 "mul rbx			\n "
			 "add rbx, r14		\n "

			 "div r15			\n "
			 "add r10, rdx		\n "
			 "mov rax, r10		\n "
			 "div r15			\n "
			 "mov r10, rdx		\n "
			 "inc r12			\n "
			 "dec rcx			\n "
			 "jmp get_hash 		\n "

		  "end_get_hash:		\n "
		  	 "mov %0, r10		\n "
		    : "=r" (hash)
		    : "r" (word), "r"(delta), "r"(length_word), "r"(SIZE_HASH_TABLE), "r"(DELTA_FOR_DELTA)
		    : "rax", "rbx", "rcx", "r10", "r11", "r12", "r14", "r15"
		);*/


		hash = asm_get_hash(word, length_word, SIZE_HASH_TABLE, DELTA_FOR_DELTA);
		
		if(!strcmp("a.m.", word))
			printf("%lld\n", hash);

	#endif

	return hash;
}

bool hash_table_is_contain_element(Hash_table_type* hash_table, const char* value, const int length, bool is_print_values) {
	if(!hash_table || !value || length <= 0) 
		return false;

	unsigned long long hash_word = get_hash_word(value, length);

	if(hash_table->chains[hash_word].size_list <= 0)
		return false;

	int is_find = 0;
	int now_pos = hash_table->chains[hash_word].head;

	for(int word = 0; word < hash_table->chains[hash_word].size_list && now_pos != hash_table->chains[hash_word].tail; ++word) {
		if(!strcmp(value, hash_table->chains[hash_word].data[now_pos].value)) {
			is_find = true;
			break;
		}

		now_pos = hash_table->chains[hash_word].data[now_pos].next;
	}

	if(is_find && is_print_values)
		print_list(&hash_table->chains[hash_word]);

	return is_find;
}

HASH_TABLE_STATUSES hash_table_insert_element_by_key(Hash_table_type* hash_table, const char* key, const int length_key, const char* value, const int length_value) {
	if(!hash_table || !hash_table->chains || !key || !value)
		return HASH_TABLE_BAD_POINTER; 

	if(length_key <= 0 || length_value <= 0)
		return HASH_TABLE_BAD_SIZE;

	unsigned long long hash_key = get_hash_word(key, length_key);
	list_insert_before(&hash_table->chains[hash_key], hash_table->chains[hash_key].size_list + 1, key, length_key, value, length_value);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES hash_table_destruct(Hash_table_type* hash_table) {
	if(!hash_table)
		return HASH_TABLE_OKEY;

	for(int list_number = 0; list_number < hash_table->size_table; ++list_number)
		free(hash_table->chains[list_number].data);

	free(hash_table->chains);

	return HASH_TABLE_OKEY;
}

HASH_TABLE_STATUSES testing_hash_table(Hash_table_type* hash_table) {
	if(!hash_table) 
		return HASH_TABLE_BAD_POINTER;


	for(int test = 0; test < COUNT_OF_TESTS; ++test) {
		int length_word = rand() % MAX_SIZE_KEY;
		char word[length_word] = "";

		for(int pos = 0; pos < length_word; ++pos) {
			word[pos] = get_random_symbol() - 'a' + 1;
		}
		
		//if(hash_table_is_contain_element(hash_table, word, length_word))

	}

	return HASH_TABLE_OKEY;
}

inline char get_random_symbol() {
	return rand() % ('z' + 1) + 'A';
}

void print_something_chain(Hash_table_type* hash_table, const unsigned long long hash) {
	for(int i = 0; i < hash_table->chains[hash].size_list; ++i)
		printf("%s\n", hash_table->chains[hash].data[i].key);
}
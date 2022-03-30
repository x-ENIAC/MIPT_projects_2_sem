#include "list.h"
#include "work_with_file.h"
#include "string.h"
#include <stdint.h>

enum HASH_TABLE_STATUSES {
	SMTH_LIST_OK                = 0,
    SMTH_LIST_ERROR             = 1,
    SMTH_LIST_EMPTY             = 2,
    SMTH_LIST_OVERFLOW          = 3,
    SMTH_LIST_CAPACITY_OVERFLOW = 4,
    SMTH_LIST_BAD_CAPACITY      = 5,
    SMTH_LIST_IS_CREATED        = 6,
    SMTH_LIST_BAD_MEMORY        = 7,
    SMTH_LIST_BAD_SIZE          = 8,
    SMTH_LIST_IS_DECTRUCT       = 9,
    SMTH_LIST_NO_CONSTRUCT      = 10,
    SMTH_LIST_DATA_NULL         = 11,
    SMTH_LIST_NO_SUCH_ELEMENT   = 12,
    SMTH_LIST_BAD_DATA          = 13,

    HASH_TABLE_OKEY				= 14,
    HASH_TABLE_BAD_POINTER		= 15,
    HASH_TABLE_BAD_SIZE         = 16,
};

const char TEXT_HASH_TABLE_STATUSES[][100] = {
	"Lists is okey",
	"Something list is error",
	"Something list is empty",
	"Something list is overflow",
	"Something list has overflow capacity",
	"Something list has bad capacity",
	"Something list is created",
	"Something list has bad memory",
	"Something list has bad size",
	"Something list is destruct",
	"Something list doesn't be construct",
	"Something list has null data",
	"Something list doesn't has such element",
	"Something list has bad data",
	"Hash table okey",
	"Hash table has bad pointer"
	/*SMTH_LIST_OK                = 0,
    SMTH_LIST_ERROR             = 1,
    SMTH_LIST_EMPTY             = 2,
    SMTH_LIST_OVERFLOW          = 3,
    SMTH_LIST_CAPACITY_OVERFLOW = 4,
    SMTH_LIST_BAD_CAPACITY      = 5,
    SMTH_LIST_IS_CREATED        = 6,
    SMTH_LIST_BAD_MEMORY        = 7,
    SMTH_LIST_BAD_SIZE          = 8,
    SMTH_LIST_IS_DECTRUCT       = 9,
    SMTH_LIST_NO_CONSTRUCT      = 10,
    SMTH_LIST_DATA_NULL         = 11,
    SMTH_LIST_NO_SUCH_ELEMENT   = 12,
    SMTH_LIST_BAD_DATA          = 13,

    HASH_TABLE_OK				= 14,
    HASH_TABLE_BAD_POINTER		= 15,*/
};

const uint32_t FIRST_BIG_NUMBER                     = 18522; // 10267;
const uint32_t SIZE_HASH_TABLE                      = 8192; // 11719;
const uint32_t DELTA_FOR_DELTA                      = 1024;
const uint32_t POWER_TWO_THAN_EQUAL_SIZE_HASH_TABLE = 13;
const uint32_t BAD_HASH                             = 0;
const uint32_t COUNT_OF_TESTS                       = 100000;
const uint32_t CONSTANT_TO_HASHING                  = 256;
const uint32_t POLYNOM_FOR_CRC32                    = 0x82f63b78;
const uint32_t SIZEOF_CHAR                          = 8;

struct Hash_table_type {
	uint32_t size_table;
	List* chains;
};

HASH_TABLE_STATUSES hash_table_construct(Hash_table_type* hash_table);

HASH_TABLE_STATUSES parsing_buffer(File* file_with_dict, Hash_table_type* hash_table);

uint32_t get_hash_word(const char* word); 

bool hash_table_is_contain_element(Hash_table_type* hash_table, const char* value, const uint32_t length, bool is_print_values = false);

HASH_TABLE_STATUSES hash_table_insert_element_by_key(Hash_table_type* hash_table, const char* key,   const uint32_t length_key, 
                                                                                  const char* value, const uint32_t length_value);

HASH_TABLE_STATUSES hash_table_destruct(Hash_table_type* hash_table);

HASH_TABLE_STATUSES testing_hash_table(Hash_table_type* hash_table);

inline char get_random_symbol();

void print_something_chain(Hash_table_type* hash_table, const uint32_t hash);


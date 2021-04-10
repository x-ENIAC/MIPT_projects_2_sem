#include "list.h"
#include "work_with_file.h"

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

const int FIRST_BIG_NUMBER  = 29;
const int SIZE_HASH_TABLE = 100;
const long long BAD_HASH = -3802;

struct Hash_table_type {
	int size_table;
	List* chains;
};

HASH_TABLE_STATUSES hash_table_construct(Hash_table_type* hash_table);

HASH_TABLE_STATUSES parsing_buffer(File* file_with_dict, Hash_table_type* hash_table);

long long get_hash_word(const char* word, const int length_word);
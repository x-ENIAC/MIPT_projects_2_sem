#include <stdio.h>
#include "warnings.h"


enum FILE_STATUS {
    FILE_OK         = 0,
    FILE_BAD_PTR    = 1,
    FILE_BAD_BUFFER = 2
};


struct File {
    const char* name_file;
    char* buffer;
    size_t size_buffer;
    //struct stat information;

    void construct(const char* file_name) {
        name_file   = file_name;  
        buffer      = NULL;
        size_buffer = 0;
    }

    FILE_STATUS resize_buffer() {
        char* new_buffer   = (char*)realloc(buffer, (size_buffer * 2 + 3) * sizeof(char));
        if(!new_buffer)
            return FILE_BAD_BUFFER;

        buffer             = new_buffer;
        size_buffer       *= 2;

        return FILE_OK;
    }

    void destroy() {
        free(buffer);
        size_buffer = 0;
    };

};

const size_t SIZE_OF_FILE_NAMES = 70;

const char TEXT_FILE_STATUS[][50] = {
    "File okey",
    "Bad ptr to file",
    "Bad buffer"
};

File file_construct(const char* name_file);

FILE_STATUS read_buffer(struct File* input_file);

size_t get_size_of_buffer(FILE* file);

FILE_STATUS file_destruct(struct File* input_file);

//#endif // WORK_WITH_FILE_H

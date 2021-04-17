#include <stdio.h>

struct File {
    const char* name_file;
    char* buffer;
    int size_buffer;
    //struct stat information;
};

enum FILE_STATUS {
    FILE_OK         = 0,
    FILE_BAD_PTR    = 1,
    FILE_BAD_BUFFER = 2
};

const int SIZE_OF_FILE_NAMES = 70;

//#ifndef WORK_WITH_FILE_H
//#define WORK_WITH_FILE_H

const char TEXT_FILE_STATUS[][50] = {
    "File okey",
    "Bad ptr to file",
    "Bad buffer"
};

File* file_construct(char* name_file);

FILE_STATUS read_buffer(struct File* input_file);

int get_size_of_buffer(FILE* file);

FILE_STATUS file_destruct(struct File* input_file);

//#endif // WORK_WITH_FILE_H

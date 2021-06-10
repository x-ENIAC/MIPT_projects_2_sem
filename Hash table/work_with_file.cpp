#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "work_with_file.h"

#define VERIFY_FILE_PTR                                                         \
        if(!file) {                                                             \
            /*warning(TEXT_FILE_STATUS[FILE_BAD_PTR], INFORMATION_ABOUT_CALL);*/    \
            return FILE_BAD_PTR;                                                \
        }

File file_construct(const char* file_name) {
    File input_file = {};
    (&input_file)->construct(file_name);;

    return input_file;
}

FILE_STATUS read_buffer(struct File* input_file) {

    //stat(input_file->name_file, &(input_file->information));

    printf("begin read buffer\n");

    FILE* file = fopen(input_file->name_file, "r");

    VERIFY_FILE_PTR

    input_file->size_buffer = get_size_of_buffer(file);
    input_file->buffer      = (char*)calloc(input_file->size_buffer + 3, sizeof(char));

    size_t status = fread(input_file->buffer, sizeof(char), input_file->size_buffer, file);

    if(status != input_file->size_buffer) {
        /*warning(TEXT_FILE_STATUS[FILE_BAD_BUFFER], INFORMATION_ABOUT_CALL);*/
        return FILE_BAD_BUFFER;
    }

    fclose(file);

    return FILE_OK;
}

size_t get_size_of_buffer(FILE* file) { // stat
    size_t length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}

FILE_STATUS resize_buffer(File* file) {
    VERIFY_FILE_PTR

    return file->resize_buffer();
}

FILE_STATUS file_destruct(File* file) {
    if(file)
        file->destroy();

    return FILE_OK;
}



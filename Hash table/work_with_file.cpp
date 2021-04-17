#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "work_with_file.h"
#include "warnings.h"

#define VERIFY_FILE_PTR                                                         \
        if(!file) {                                                             \
            warning(TEXT_FILE_STATUS[FILE_BAD_PTR], INFORMATION_ABOUT_CALL);    \
            return FILE_BAD_PTR;                                                \
        }

File* file_construct(char* file_name) {
    File* input_file = {};

    printf("%s\n111\n", file_name);
    input_file->name_file   = file_name;    printf("222\n");

    input_file->buffer      = NULL;
    input_file->size_buffer = 0;

    return input_file;
}

FILE_STATUS read_buffer(struct File* input_file) {

    //stat(input_file->name_file, &(input_file->information));

    printf("begin read buffer\n");

    FILE* file = fopen(input_file->name_file, "r");

    VERIFY_FILE_PTR

    input_file->size_buffer = get_size_of_buffer(file);
    input_file->buffer      = (char*)calloc(input_file->size_buffer + 3, sizeof(char));

    int status = fread(input_file->buffer, sizeof(char), input_file->size_buffer, file);
    if(status != input_file->size_buffer) {
        warning(TEXT_FILE_STATUS[FILE_BAD_BUFFER], INFORMATION_ABOUT_CALL);
        return FILE_BAD_BUFFER;
    }

    fclose(file);
    return FILE_OK;
}

int get_size_of_buffer(FILE* file) { // stat
    int length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}

FILE_STATUS resize_buffer(struct File* file) {
    VERIFY_FILE_PTR

    char* new_buffer = (char*)realloc(file->buffer, file->size_buffer * 2 * sizeof(char));

    file->buffer = new_buffer;
    file->size_buffer *= 2;

    return FILE_OK;
}

FILE_STATUS file_destruct(struct File* file) {
    if(file) {
        //free(file->name_file);
        free(file->buffer);
    }

    free(file);

    return FILE_OK;
}



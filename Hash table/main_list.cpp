/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 11, 2020, 10:17:25
 *  @par What was changed?
 *                      1. Fixed tests
 *                      2. Fixed search physical position by logical
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define FIRST_TEST

int main() {

    char* ptr = (char*)calloc(6, sizeof(char));
    ptr[0] = 'N';
    ptr[1] = 'I';
    ptr[2] = 'C';
    ptr[3] = 'E';
    ptr[4] = 'D';
    ptr[5] = 'A';
    ptr[6] = 'Y';

    FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);

    #ifdef FIRST_TEST
    {
        List my_list = {0};
        list_construct(&my_list);
        print_list(&my_list);
        
        list_insert_before(&my_list, 1, ptr, 7);
        print_list(&my_list);

        ptr[0] = 'C'; ptr[1] = 'A'; ptr[2] = 'T';
        list_insert_before(&my_list, 2, ptr, 3);
        print_list(&my_list);

        ptr[0] = 'S'; ptr[1] = 'U'; ptr[2] = 'N'; ptr[3] = 'N';
        list_insert_before(&my_list, 3, ptr, 4);
        print_list(&my_list);

        ptr[0] = 'S'; ptr[1] = 'U'; ptr[2] = 'N'; ptr[3] = 'N';
        list_insert_before(&my_list, 4, ptr, 4);

        ptr[0] = 'C'; ptr[1] = 'A'; ptr[2] = 'T';
        list_insert_before(&my_list, 5, ptr, 3);

        ptr[0] = 'S'; ptr[1] = 'U'; ptr[2] = 'N'; ptr[3] = 'N';
        list_insert_before(&my_list, 6, ptr, 4);
        print_list(&my_list);

        ptr[0] = 'Q'; ptr[1] = 'U'; ptr[2] = 'E'; ptr[3] = 'E'; ptr[4] = 'N';
        list_insert_before(&my_list, 7, ptr, 5);  

        print_list(&my_list);

    }
    #endif // FIRST_TEST

    return 0;
}

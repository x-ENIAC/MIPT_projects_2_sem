#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include "list.h"
#include <string.h>

//#define BEAUTIFUL_PRINT
#define IF_DEBUG(code) code

#define REPORT_ABOUT_ERROR(code_of_error)                                                            \
    my_list->list_status = code_of_error;                                                            \
    /*list_warning(my_list, INFORMATION_ABOUT_CALL); */                                                       \
    list_dump(my_list, INFORMATION_ABOUT_CALL);

#define VERIFY_LIST_STATUS                                                                           \
    if(status != LIST_OK) {                                                                          \
        REPORT_ABOUT_ERROR(status);                                                                  \
        return status;                                                                               \
    }

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// DRAW AND DUMP FUNCTIONS /////////////////////////////////////////////////////////////////

void list_dump(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    FILE* log_file = fopen(name_input_html_file, "a");

    fprintf(log_file, "<pre><tt>\n");
    fprintf(log_file, "    size_list:   %lu\n",      my_list->size_list);
    fprintf(log_file, "     capacity:   %lu\n",      my_list->capacity);
    fprintf(log_file, "       status:   %d (%s)\n",  my_list->list_status, TEXT_LIST_STATUSES[my_list->list_status]);
    fprintf(log_file, "         head:   %lu\n",      my_list->head);
    fprintf(log_file, "         tail:   %lu\n",      my_list->tail);
    fprintf(log_file, " nearest_free:   %lu\n",      my_list->nearest_free);
    fprintf(log_file, "number_of_call:  %d\n",       my_list->number_of_draw_graph_call);
    fprintf(log_file, "Dump call has occured in %s, in function %s, line %d\n", arguments_of_call.name_file, arguments_of_call.name_function, arguments_of_call.number_of_line);

    file_print_list(log_file, my_list);

    draw_graph(my_list);
    fprintf(log_file, "\n\n<img src=picture%d.png/>\n\n", my_list->number_of_draw_graph_call - 1);

    fclose(log_file);
}

void draw_graph(List* my_list) {
    FILE* file = fopen(concatenate_strings_for_draw_graph(my_list), "wb");
    fprintf(file, "digraph graphname {\nrankdir = LR;\n");

    draw_fictive_element(file, my_list);
    draw_all_elements(file, my_list);
    draw_list_elements(file, my_list);
    draw_free_elements(file, my_list);

    char result_string[SIZE_OF_NAME_FILES] = "";
    sprintf(result_string, "dot picture%d.dot -T png -o picture%d.png", my_list->number_of_draw_graph_call, my_list->number_of_draw_graph_call); // | eog picture0.png"; //, my_list->number_of_draw_graph_call);

    ++my_list->number_of_draw_graph_call;

    fclose(file);

    system(result_string); // "dot new.dot -Tpng -O" //get_string_to_draw_graph(my_list)
}

void draw_fictive_element(FILE* file, List* my_list) {
    fprintf(file, "\"Fictive element\" [shape=\"record\", color=\"%s\", style=\"filled\", fillcolor=\"%s\"]\n", CONTOUR_LABEL_COLOR_FOR_FICTIVE_ELEMENT, INTERIOR_LABEL_COLOR_FOR_FICTIVE_ELEMENT);

    fprintf(file, "\"Fictive element\"->node%lu [color=\"%s\", label=\"head\"]\n",         my_list->head,         ARROW_COLOR_FOR_FICTIVE_ELEMENT);
    fprintf(file, "\"Fictive element\"->node%lu [color=\"%s\", label=\"tail\"]\n",         my_list->tail,         ARROW_COLOR_FOR_FICTIVE_ELEMENT);
    fprintf(file, "\"Fictive element\"->node%lu [color=\"%s\", label=\"nearest free\"]\n", my_list->nearest_free, ARROW_COLOR_FOR_FICTIVE_ELEMENT);
}

void draw_all_elements(FILE* file, List* my_list) {
    for(size_t i=my_list->fictive+1; i<my_list->capacity; ++i) {
        fprintf(file, "node%lu [shape=\"record\", ", i);

        if(!my_list->data[i].is_used) {
            fprintf(file, "color=\"%s\", style=\"filled\", fillcolor=\"%s\", ", CONTOUR_LABEL_COLOR_FOR_UNVALID_ELEMENTS, INTERIOR_LABEL_COLOR_FOR_UNVALID_ELEMENTS);
        } else {
            fprintf(file, "color=\"%s\", style=\"filled\", fillcolor=\"%s\", ", CONTOUR_LABEL_COLOR_FOR_VALID_ELEMENTS, INTERIOR_LABEL_COLOR_FOR_VALID_ELEMENTS);
        }

        fprintf(file, "label=\"%lu|{%lu|%lu}|%d\"]\n", i, my_list->data[i].prev, my_list->data[i].next, my_list->data[i].value);

        if(i + 1 < my_list->capacity) {
            fprintf(file, "node%lu->node%lu [color=%s, style=\"dashed\"]\n", i, i+1, ARROW_COLOR_FOR_UNVALID_ELEMENTS);
        }
    }

    fprintf(file, "node%lu->node%lu [color=%s, style=\"dashed\"]\n", my_list->capacity - 1, my_list->fictive + 1, ARROW_COLOR_FOR_UNVALID_ELEMENTS);
}

void draw_list_elements(FILE* file, List* my_list) {
    size_t now_position = my_list->head;

    if(my_list->size_list > 0) {

        fprintf(file, "node%lu", now_position);
        if(my_list->size_list > 1) {
            now_position = my_list->data[now_position].next;

            while(now_position != my_list->head) {
                fprintf(file, "->node%lu", now_position);
                now_position = my_list->data[now_position].next;

            }
        }

        fprintf(file, "->node%lu [color=%s]\n", now_position, ARROW_COLOR_FOR_VALID_ELEMENTS);
    }
}

void draw_free_elements(FILE* file, List* my_list) {
    size_t now_position = my_list->nearest_free;
    fprintf(file, "node%lu", now_position);

    while(my_list->data[now_position].next < my_list->capacity) {
        now_position = my_list->data[now_position].next;
        fprintf(file, "->node%lu", now_position);
    }

    fprintf(file, "->node%lu [color=%s]\n}\n", my_list->nearest_free, ARROW_COLOR_FOR_FREE_ELEMENTS);
}

char* concatenate_strings_for_draw_graph(List* my_list) {
    char* result_string = (char*)calloc(SIZE_OF_NAME_FILES, sizeof(char));

    sprintf(result_string, "picture%d.dot", my_list->number_of_draw_graph_call);

    return result_string;
}

void file_print_list(FILE* log_file, List* my_list) {
    fprintf(log_file, "data\n ");
    fprintf(log_file, "\tvalue:\t ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%8d ", my_list->data[i].value);
    }
    fprintf(log_file, "\n\tnext:\t ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%8lu ", my_list->data[i].next);
    }
    fprintf(log_file, "\n\tprev:\t ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%8lu ", my_list->data[i].prev);
    }
    fprintf(log_file, "\n\tis_used: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%8d ", my_list->data[i].is_used);
    }
    fprintf(log_file, "\n\n");
}*/

static void list_dump(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {}

void print_list(List* my_list) {
    #ifdef BEAUTIFUL_PRINT 
        printf("data\n ");
        printf("%ld, head: %ld, tail: %ld, nearest free: %ld\n", my_list->size_list, my_list->head, my_list->tail, my_list->nearest_free);
        printf("\tvalue: ");
        for(size_t i=0; i<my_list->capacity; ++i) {
            printf(" ");
            for(size_t j = 0; j < my_list->data[i].length; ++j)
                printf( "%c", my_list->data[i].value[j]);
            //printf("%s", my_list->data[i].value);
        }
        printf("\n\tlength: ");
        for(size_t i = 0; i < my_list->capacity; ++i) {
            printf("%4d ", my_list->data[i].length);
        }    
        printf("\n\tnext: ");
        for(size_t i = 0; i < my_list->capacity; ++i) {
            printf("%4lu ", my_list->data[i].next);
        }
        printf("\n\tprev: ");
        for(size_t i = 0; i < my_list->capacity; ++i) {
            printf("%4lu ", my_list->data[i].prev);
        }
        printf("\n\tis_used: ");
        for(size_t i = 0; i < my_list->capacity; ++i) {
            printf("%4d ", my_list->data[i].is_used);
        }
        printf("\n\n");
    #else
        for(size_t i = 0; i < my_list->capacity; ++i) {
            printf("\n|");
            for(size_t j = 0; j < my_list->data[i].length_value; ++j) {
                printf("%c", my_list->data[i].value[j]);
            }
        }

    #endif

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// WORK WITH LIST /////////////////////////////////////////////////////////////////

static void list_verifier(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    if(nullptr == my_list) {
        my_list->list_status = LIST_DATA_NULL;
        list_dump(my_list, arguments_of_call);
    } else if((my_list->size_list) > (my_list->capacity)) {
        my_list->list_status = LIST_OVERFLOW;
        list_dump(my_list, arguments_of_call);
    } /*else if(my_list->size_list < 0) {
        my_list->list_status = LIST_BAD_SIZE;
        list_dump(my_list, arguments_of_call);
    } else if(my_list->capacity < 0) {
        my_list->list_status = LIST_BAD_CAPACITY;
        list_dump(my_list, arguments_of_call);
    }*/ else if(my_list->list_status != LIST_OK) {
        list_dump(my_list, arguments_of_call);
    }

    size_t prev_position = 0, next_position = 0;

    for(size_t now_position=my_list->head; now_position!=my_list->head; now_position = my_list->data[now_position].next) {
        prev_position = (my_list->data[now_position].prev + my_list->size_list) % my_list->size_list;
        next_position = (my_list->data[now_position].next + my_list->size_list) % my_list->size_list;

        if((my_list->data[prev_position].next + my_list->size_list) % my_list->size_list != now_position  ||
           (my_list->data[next_position].prev + my_list->size_list) % my_list->size_list != now_position) {
            my_list->list_status = LIST_BAD_DATA;
            list_dump(my_list, arguments_of_call);
           }
    }
}

static void list_initializate(List* my_list, const size_t begin_position) {

    for(size_t now_position = begin_position; now_position <= my_list->capacity; ++now_position) {
        //my_list->data[now_position].value   = ""; // (char*)calloc(MAX_SIZE_WORD, sizeof(char));
        //my_list->data[now_position].key     = ""; // (char*)calloc(MAX_SIZE_WORD, sizeof(char));
        memcpy(my_list->data[now_position].value, POISON, LENGTH_POISON - 1);
        my_list->data[now_position].length_value  = LENGTH_POISON;

        my_list->data[now_position].next    = now_position + 1;
        my_list->data[now_position].prev    = now_position - 1;
        my_list->data[now_position].is_used = false;
    }

    if(begin_position == 0) {
        my_list->data[0].prev = 0;
        memcpy(my_list->data[0].value, FICTIVE, LENGTH_FICTIVE - 1);
        my_list->data[0].length_value  = LENGTH_FICTIVE;
    }
}

LIST_STATUSES list_construct(List* my_list, const size_t capacity) {
    if(!my_list) {
        REPORT_ABOUT_ERROR(LIST_NO_CONSTRUCT)
        return LIST_NO_CONSTRUCT;
    }

    my_list->data = (Node*)calloc(capacity + 1, sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return LIST_BAD_MEMORY;
    }

    my_list->capacity  = capacity;
    my_list->size_list = 0;

    list_initializate(my_list, 0);

    my_list->head                      = 0;
    my_list->tail                      = 0;
    my_list->flag_of_sorted            = true;
    my_list->nearest_free              = 1;
    my_list->number_of_draw_graph_call = 0;
    my_list->fictive                   = 0;

    my_list->list_status = LIST_OK;

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

static size_t get_min_free_position(List* my_list) {
    size_t now_position = my_list->nearest_free;

    while(!my_list->data[my_list->data[now_position].prev].is_used && now_position > my_list->data[now_position].prev) {
        now_position = my_list->data[now_position].prev;
    }

    return now_position;
}

LIST_STATUSES list_insert_after(List* my_list, const size_t physical_position, const char* key,   const size_t length_key, 
                                                                               const char* value, const size_t length_value) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(physical_position > my_list->size_list) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT);
        return LIST_NO_SUCH_ELEMENT;
    }

    return list_insert_before(my_list, my_list->data[physical_position].next, key, length_key, value, length_value);
}

static LIST_STATUSES list_resize(List* my_list, const double quantity) {
    //printf("%llu  :   %lg\n", my_list->capacity, quantity);
    if((size_t)(quantity * my_list->capacity) > MAX_VALUE_SIZE_T || (size_t)quantity < 0) {
        REPORT_ABOUT_ERROR(LIST_OVERFLOW)
    }
    //printf("%llu  :   %lg\n", my_list->capacity, quantity);

    size_t new_capacity = my_list->capacity * (size_t)quantity;
    size_t old_capacity = my_list->capacity;

    my_list->data = (Node*)realloc(my_list->data, (new_capacity + 1) * sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return LIST_BAD_MEMORY;
    }

    my_list->capacity = new_capacity;

    if(quantity > 1) {
        list_initializate(my_list, old_capacity + 1);
    }

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

static LIST_STATUSES list_insert_first_element(List* my_list, const size_t temporary_free, const char* key,   const size_t length_key, 
                                                                                           const char* value, const size_t length_value) {
    my_list->head = temporary_free;
    my_list->nearest_free                = my_list->data[temporary_free].next;

    for(size_t j = 0; j < length_value; ++j) 
        my_list->data[my_list->head].value[j] = value[j];

    my_list->data[my_list->head].length_value  = length_value;
    my_list->data[my_list->head].next          = my_list->head;
    my_list->data[my_list->head].prev          = my_list->tail;
    my_list->data[my_list->head].is_used       = true;
    my_list->tail                              = temporary_free;

    my_list->data[temporary_free].length_value   = (length_value + 1 >= MAX_SIZE_VALUE ? MAX_SIZE_VALUE - 1 : length_value);
    memcpy(my_list->data[temporary_free].value, value, length_value);

    my_list->data[temporary_free].length_key   = (length_key + 1 >= MAX_SIZE_KEY ? MAX_SIZE_KEY - 1 : length_key);
    memcpy(my_list->data[temporary_free].key, key, length_key);
      

    ++(my_list->size_list);

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

LIST_STATUSES list_insert_before(List* my_list, const size_t physical_position, const char* key,   const size_t length_key, 
                                                                                const char* value, const size_t length_value) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(0 == length_value  || 0 == length_key)
        return LIST_ERROR;

    if(my_list->size_list + 2 >= my_list->capacity) {
        LIST_STATUSES status = list_resize(my_list, 2);
        VERIFY_LIST_STATUS
    }

    if(physical_position != my_list->size_list) {
        my_list->flag_of_sorted = false;
    }

    size_t temporary_free = my_list->nearest_free;
    my_list->nearest_free = my_list->data[my_list->nearest_free].next;

    if(0 == my_list->size_list) {
        return list_insert_first_element(my_list, temporary_free, key, length_key, value, length_value);
    }

    //printf("!!!\n");
    my_list->data[temporary_free].length_value = (length_value + 1 > MAX_SIZE_VALUE ? MAX_SIZE_VALUE : length_value + 1); 
    size_t amount = my_list->data[temporary_free].length_value - 1;   
    //printf(")))\n");

    memcpy(my_list->data[temporary_free].value, value, amount * sizeof(char));

    //printf("&&&\n");
    my_list->data[temporary_free].length_key = MAX_SIZE_KEY; //(length_key + 1 > MAX_SIZE_KEY ? MAX_SIZE_KEY : length_key + 1); 
    //amount = my_list->data[temporary_free].length_key - 1; 
    //printf("length key: %lu\n", my_list->data[temporary_free].length_key);
      
    //printf("(((\n");
    //printf("length key: %d\n\n", my_list->data[temporary_free].length_key);
    //memset(my_list->data[temporary_free].key, 0, MAX_SIZE_KEY * sizeof(char));
    memcpy(my_list->data[temporary_free].key, key, length_key * sizeof(char));

    //printf("???\n");
    if(0 != my_list->data[physical_position].prev)
        my_list->data[my_list->data[physical_position].prev].next = temporary_free;

    my_list->data[temporary_free].next = physical_position;
    my_list->data[temporary_free].prev = my_list->data[physical_position].prev;

    if(physical_position != temporary_free) {
        my_list->data[physical_position].prev = temporary_free;
    }

    if(my_list->head == physical_position) {
        my_list->data[temporary_free].prev = my_list->tail;
        my_list->data[my_list->tail].next = temporary_free;
        my_list->head = temporary_free;
    }

    if(physical_position == my_list->size_list + 1) {
        my_list->tail = temporary_free;
        my_list->data[temporary_free].next = my_list->head;
        my_list->data[my_list->head].prev = my_list->tail;
    }

    my_list->data[temporary_free].is_used = true;
    my_list->nearest_free = get_min_free_position(my_list);
    ++(my_list->size_list);

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    //print_list(my_list);

    //printf("!!!\n\n");

    return LIST_OK;
}

/*
LIST_STATUSES list_insert_back(List* my_list, Elem_type value) {
    LIST_STATUSES status = list_insert_before(my_list, my_list->size_list + 1, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

LIST_STATUSES list_insert_front(List* my_list, const Elem_type value) {
    LIST_STATUSES status = list_insert_before(my_list, 1, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}
*/

bool list_find_element(List* my_list, const char* value, const size_t length) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    size_t pointer = my_list->head;
    bool is_equal = false;

    while(pointer != my_list->tail) {
        if(length == my_list->data[pointer].length_value) {
            is_equal = true;
            for(size_t j = 0; j < length; ++j) {
                if(my_list->data[pointer].value[j] != value[j]) {
                    is_equal = false;
                    break;
                }
            }

        }
        if(is_equal)
            break;
    }

    return is_equal;
}
/*
LIST_STATUSES list_delete_element(List* my_list, const size_t position, Elem_type* position_delete_value) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(position < 0) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        *position_delete_value = my_list->fictive;
        return LIST_NO_SUCH_ELEMENT;
    }

    if(my_list->size_list * 4 <= my_list->capacity) {
        list_resize(my_list, 0.25);
    }

    if(my_list->size_list != position) {
        my_list->flag_of_sorted = false;
    }

    *position_delete_value = position;

    if(my_list->head == position) {
        my_list->head = my_list->data[my_list->head].next;
    }

    if(my_list->tail == position) {
        my_list->tail = my_list->data[my_list->tail].prev;
    }

    my_list->data[my_list->data[position].prev].next = my_list->data[position].next;
    my_list->data[my_list->data[position].next].prev = my_list->data[position].prev;

    my_list->data[position].is_used = false;

    put_free_position(my_list, position);

    --my_list->size_list;

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

LIST_STATUSES list_pop_front(List* my_list, Elem_type* value) {
    LIST_STATUSES status = list_delete_element(my_list, my_list->head, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}

LIST_STATUSES list_pop_back(List* my_list, Elem_type* value) {
    LIST_STATUSES status = list_delete_element(my_list, my_list->tail, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    return LIST_OK;
}


void put_free_position(List* my_list, const size_t position) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(position >= my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return;
    }

    size_t now_position = my_list->nearest_free;

    while(now_position < position) {
        now_position = my_list->data[now_position].next;
    }

    size_t prev_position = my_list->data[now_position].prev;
    size_t next_position = now_position;

    size_t prev_of_next_position = my_list->data[next_position].prev;

    my_list->data[next_position].prev = position;
    my_list->data[position].next = next_position;

    if(!my_list->data[prev_of_next_position].is_used) {
        my_list->data[prev_position].next = next_position;
        my_list->data[next_position].prev = prev_position;
    }

    my_list->nearest_free = get_min_free_position(my_list);
}*/


void list_destruct(List* my_list) {
    free(my_list->data);

    my_list->capacity  = INT_POISON;
    my_list->size_list = INT_POISON;
    my_list->head      = INT_POISON;
    my_list->tail      = INT_POISON;
}

/*

size_t get_max_free_position(List* my_list) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    size_t now_position = my_list->nearest_free;

    while(!my_list->data[now_position].is_used && my_list->data[now_position].next < my_list->capacity) {
        now_position = my_list->data[now_position].next;
    }

    return now_position;
}

size_t list_slow_find_physical_position_by_logical(List* my_list, const size_t position) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(position > my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return my_list->fictive;
    }

    if(!my_list->data[position].is_used) {
        my_list->list_status = LIST_NO_SUCH_ELEMENT;
        list_warning(my_list, INFORMATION_ABOUT_CALL);
        return my_list->fictive;
    }

    if(my_list->flag_of_sorted) {
        return position;
    }

    size_t begin_position = my_list->head;
    for(size_t number_of_element = 0; number_of_element < position; ++number_of_element) {
        if(my_list->data[begin_position].next >= my_list->capacity) {
            my_list->list_status = LIST_NO_SUCH_ELEMENT;
            list_warning(my_list, INFORMATION_ABOUT_CALL);
            return my_list->fictive;
        }

        begin_position = my_list->data[begin_position].next;
    }

    return begin_position;
}

LIST_STATUSES list_very_very_slow_sort(List* my_list) {
    IF_DEBUG(list_verifier(my_list, INFORMATION_ABOUT_CALL);)

    if(my_list->flag_of_sorted)
        return LIST_OK;

    List sorted_list_by_next = {0};
    list_construct(&sorted_list_by_next, my_list->capacity);

    size_t now_position = 0;

    for(size_t i=0; i<=my_list->size_list; ++i) {
        sorted_list_by_next.data[i].value   = my_list->data[now_position].value;
        now_position = my_list->data[now_position].next;
    }

    for(size_t i=my_list->fictive + 1; i<=my_list->size_list; ++i) {
        my_list->data[i].value   = sorted_list_by_next.data[i].value;
        my_list->data[i].is_used = true;
        my_list->data[i].next    = i + 1;
        my_list->data[i].prev    = i - 1;
    }

    list_initializate(my_list, my_list->size_list + 1);

    my_list->flag_of_sorted = true;
    my_list->head           = my_list->fictive   + 1;
    my_list->tail           = my_list->size_list;

    my_list->data[my_list->head].prev = my_list->tail;
    my_list->data[my_list->tail].next = my_list->head;

    return LIST_OK;
}*/

void list_warning(const List* my_list, struct call_of_dump arguments_of_call) {
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");
    char warning_info[SIZE_OF_LIST_WARNINGS] = "";
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %lu)\\e[0m\"", arguments_of_call.name_file, arguments_of_call.name_function, arguments_of_call.number_of_line);
    system(warning_info);
    sprintf(warning_info, "echo \"\\e[31mList status: %s\\e[0m\"", TEXT_LIST_STATUSES[my_list->list_status]);
    system(warning_info);
}

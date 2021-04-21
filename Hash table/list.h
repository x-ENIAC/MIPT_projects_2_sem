#include <stdio.h>
#include "warnings.h"

#define Elem_type char*

const size_t       MAX_SIZE_WORD            =        300;
const size_t       MAX_SIZE_KEY             =         64;
const size_t       MAX_SIZE_VALUE           =        300;
const size_t       INT_POISON               =      -3802;
const size_t       BEGIN_INIT_SIZE          =          8;
const size_t       SIZE_OF_NAME_FILES       =         50;
const size_t       SIZE_OF_LIST_WARNINGS    =        150;
const size_t       MAX_VALUE_SIZE_T         = (size_t)-1;

const char         POISON[8]                = "POISON";
const size_t       LENGTH_POISON            = 6;


const char         FICTIVE[8]               = "FICTIVE";
const size_t       LENGTH_FICTIVE           = 7;

const char name_input_html_file[]           = "list_dump.html"; 

typedef enum {
    LIST_OK                = 0,
    LIST_ERROR             = 1,
    LIST_EMPTY             = 2,
    LIST_OVERFLOW          = 3,
    LIST_CAPACITY_OVERFLOW = 4,
    LIST_BAD_CAPACITY      = 5,
    LIST_IS_CREATED        = 6,
    LIST_BAD_MEMORY        = 7,
    LIST_BAD_SIZE          = 8,
    LIST_IS_DECTRUCT       = 9,
    LIST_NO_CONSTRUCT      = 10,
    LIST_DATA_NULL         = 11,
    LIST_NO_SUCH_ELEMENT   = 12,
    LIST_BAD_DATA          = 13,
} LIST_STATUSES;

const char TEXT_LIST_STATUSES[][80] {
    "List is okey",
    "Error!",
    "List is empty",
    "Size of list is very big",
    "Capacity of list is very big",
    "Capacity is lower then size",
    "List is created now",
    "Data is null",
    "Size is bad",
    "List is destructed",
    "List don't construct",
    "Data is null",
    "No such element in list",
    "Data is bad (maybe, there was a confusion with pointers)"
};

struct Node {
    size_t prev;
    size_t next;
    bool is_used;
    //Elem_type value;
    char value[MAX_SIZE_VALUE];
    size_t length_value;

    //Elem_type key;
    char key[MAX_SIZE_KEY];
    size_t length_key;    
};

struct List {
    Node* data;
    size_t capacity;
    size_t size_list;
    size_t head;
    size_t tail;
    size_t fictive;
    LIST_STATUSES list_status;
    bool flag_of_sorted;
    size_t nearest_free;
    size_t number_of_draw_graph_call;
};

const char CONTOUR_LABEL_COLOR_FOR_VALID_ELEMENTS[]   = "blue";
const char CONTOUR_LABEL_COLOR_FOR_UNVALID_ELEMENTS[] = "red";
const char CONTOUR_LABEL_COLOR_FOR_FICTIVE_ELEMENT[]  = "darkolivegreen";

const char INTERIOR_LABEL_COLOR_FOR_VALID_ELEMENTS[]   = "lightcyan2";
const char INTERIOR_LABEL_COLOR_FOR_UNVALID_ELEMENTS[] = "lavenderblush";
const char INTERIOR_LABEL_COLOR_FOR_FICTIVE_ELEMENT[]  = "palegreen";

const char ARROW_COLOR_FOR_VALID_ELEMENTS[]   = "purple";
const char ARROW_COLOR_FOR_UNVALID_ELEMENTS[] = "green";
const char ARROW_COLOR_FOR_FREE_ELEMENTS[]    = "black";
const char ARROW_COLOR_FOR_FICTIVE_ELEMENT[]  = "maroon";

void                    delete_old_information_from_file                       ();

struct call_of_dump     create_struct                                          (const char* file_name, size_t number, const char* function_name);

//static void             list_dump                                              (List* my_list, struct call_of_dump arguments_of_call);

//static void             list_verifier                                          (List* my_list);

void                    draw_graph                                             (List* my_list);

void                    draw_fictive_element                                   (FILE* file, List* my_list);

void                    draw_all_elements                                      (FILE* file, List* my_list);

void                    draw_list_elements                                     (FILE* file, List* my_list);

void                    draw_free_elements                                     (FILE* file, List* my_list);

char*                   get_string_to_draw_graph                               (List* my_list);

char*                   concatenate_strings_for_draw_graph                     (List* my_list);

void                    file_print_list                                        (FILE* log_file, List* my_list);

void                    list_print                                             (List* my_list);

void                    print_list                                             (List* my_list);

LIST_STATUSES           list_construct                                         (List* my_list, const size_t capacity = BEGIN_INIT_SIZE);

//static void             list_initializate                                      (List* my_list, const size_t begin_position = 0);

void                    list_destruct                                          (List* my_list);

LIST_STATUSES           list_insert_before                                     (List* my_list, const size_t physical_position, const char* key, const size_t length_key, const char* value, const size_t length_value);

LIST_STATUSES           list_insert_after                                      (List* my_list, const size_t physical_position, const char* key, const size_t length_key, const char* value, const size_t length);

//static LIST_STATUSES    list_resize                                            (List* my_list, const double quantity);

bool                    list_find_element                                      (List* my_list, const char* value, const size_t length);

//static LIST_STATUSES    list_insert_first_element                              (List* my_list, const size_t temporary_free, const char* key, const size_t length_key, const char* value, const size_t length);

void                    put_free_position                                      (List* my_list, const size_t position);

//static size_t           get_min_free_position                                  (List* my_list);

void                    list_warning                                           (List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call);

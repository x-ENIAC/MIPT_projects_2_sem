#include <stdio.h>

#define Elem_type char*
#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

//const Elem_type POISON              =       NULL;
const int       MAX_SIZE_WORD       =        100;
const int       POISON              =      -3802;
const int       BEGIN_INIT_SIZE     =          8;
const int       SIZE_OF_NAME_FILES  =         50;
const int       SIZE_OF_WARNINGS    =        150;
const size_t    MAX_VALUE_SIZE_T    = (size_t)-1;

const char name_input_html_file[] = "list_dump.html";

struct call_of_dump {
    const char* name_file;
    int number_of_line;
    const char* name_function;
};

const struct call_of_dump base_arguments_of_call = {__FILE__, -1, " "};

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
    Elem_type value;
    int length;
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
    int number_of_draw_graph_call;
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

struct call_of_dump     create_struct                                          (const char* file_name, int number, const char* function_name);

void                    list_dump                                              (List* my_list, struct call_of_dump arguments_of_call);

void                    list_verifier                                          (List* my_list);

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

void                    list_initializate                                      (List* my_list, const size_t begin_position = 0);

void                    list_destruct                                          (List* my_list);

//LIST_STATUSES           list_insert_before                                     (List* my_list, const size_t physical_position, const Elem_type value);
LIST_STATUSES           list_insert_before                                     (List* my_list, const size_t physical_position, const char* value, const int length);

//LIST_STATUSES           list_insert_after                                      (List* my_list, const size_t physical_position, const Elem_type value);
LIST_STATUSES           list_insert_after                                      (List* my_list, const size_t physical_position, const char* value, const int length);

LIST_STATUSES           list_resize                                            (List* my_list, const double quantity);

//LIST_STATUSES           list_insert_first_element                              (List* my_list, const size_t temporary_free, const Elem_type value);
LIST_STATUSES           list_insert_first_element                              (List* my_list, const size_t temporary_free,  const char* value, const int length);

LIST_STATUSES           list_insert_front                                      (List* my_list, const Elem_type value);

LIST_STATUSES           list_insert_back                                       (List* my_list, const Elem_type value);

LIST_STATUSES           list_delete_element                                    (List* my_list, const size_t position, Elem_type* delete_value);

LIST_STATUSES           list_pop_front                                         (List* my_list, Elem_type* value);

LIST_STATUSES           list_pop_back                                          (List* my_list, Elem_type* value);

void                    put_free_position                                      (List* my_list, const size_t position);

size_t                  get_min_free_position                                  (List* my_list);

size_t                  get_max_free_position                                  (List* my_list);

Elem_type               list_slow_find_physical_position_by_logical            (List* my_list, const size_t position);

LIST_STATUSES           list_very_very_slow_sort                               (List* my_list);

void                    warning                                                (List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call);

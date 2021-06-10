#ifndef WARNINGS_H
#define WARNINGS_H

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, (size_t)__LINE__, __FUNCTION__}

const size_t SIZE_OF_WARNINGS = 70;

struct call_of_dump {
    const char* name_file;
    size_t number_of_line;
    const char* name_function;
};

const struct call_of_dump base_arguments_of_call = {__FILE__, 0, " "};

void warning(const char* information, struct call_of_dump arguments_of_call);

#endif // WARNINGS_H

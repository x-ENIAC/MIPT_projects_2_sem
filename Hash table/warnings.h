#ifndef WARNINGS_H
#define WARNINGS_H

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

const int SIZE_OF_WARNINGS = 70;

struct call_of_dump {
    const char* name_file;
    int number_of_line;
    const char* name_function;
};

void warning(const char* information, struct call_of_dump arguments_of_call);

#endif // WARNINGS_H

#ifndef UTIL_H_
#define UTIL_H_

void* check_malloc(int sz); 
void Fatal(const char* format, ...);

#define CHECK_WITH_MSG(condition, msg, ...)       \
    do {                                          \
       if (!(condition)) Fatal(msg, __VA_ARGS__); \
    } while(false);

#define CHECK(condition) CHECK_WITH_MSG(condition, #condition)
#endif
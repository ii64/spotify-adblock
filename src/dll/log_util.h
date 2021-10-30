
#ifndef _LOG_UTIL_H
#define _LOG_UTIL_H

#include "lib/log.c/src/log.h"

#define STRINGIZE(arg) STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCAT(arg1, arg2) CONCAT1(arg1, arg2)
#define CONCAT1(arg1, arg2) CONCAT2(arg1, arg2)
#define CONCAT2(arg1, arg2) arg1##arg2

#define log_var_offsets_1(log_t, fmt, var, ...) \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var);
#define log_var_offsets_2(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_1(log_t, __VA_ARGS__);
#define log_var_offsets_3(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_2(log_t, __VA_ARGS__);
#define log_var_offsets_4(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_3(log_t, __VA_ARGS__);
#define log_var_offsets_5(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_4(log_t, __VA_ARGS__);
#define log_var_offsets_6(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_5(log_t, __VA_ARGS__);
#define log_var_offsets_7(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_6(log_t, __VA_ARGS__);
#define log_var_offsets_8(log_t, fmt, var, ...)                       \
    log_log(log_t, __FILE__, __LINE__, STRINGIZE(var) ": " fmt, var); \
    log_var_offsets_7(log_t, __VA_ARGS__);

#define log_var_offsets_narg(...) log_var_offsets_narg_(__VA_ARGS, log_var_offsets_rseq_n())
#define log_var_offsets_narg_(...) log_var_offsets_arg_n(__VA_ARGS__)
#define log_var_offsets_arg_n(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define log_var_offsets_rseq_n() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define log_var_(log_t, N, fmt, var, ...) \
    CONCAT(log_var_offsets_, N)           \
    (log_t, fmt, var, __VA_ARGS__)
#define log_var(log_t, fmt, var, ...) \
    log_var_(log_t, log_var_offsets_narg(fmt, var, __VA_ARGS__), fmt, var, __VA_ARGS__)

#endif
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef enum {
    INPUT_RANDOM = 1,
    INPUT_SORTED,
    INPUT_REVERSE,
    INPUT_MANY_DUPLICATES
} InputPattern;

const char *pattern_name(InputPattern pattern);
int *generate_array(size_t n, InputPattern pattern, unsigned int seed);
int *copy_array(const int *src, size_t n);
int is_sorted_asc(const int *arr, size_t n);
void clear_stdin(void);
int read_int_in_range(const char *prompt, int min_val, int max_val);
size_t read_size_t_in_range(const char *prompt, size_t min_val, size_t max_val);

#endif /* UTILS_H */

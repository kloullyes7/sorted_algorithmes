#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *pattern_name(InputPattern pattern) {
    switch (pattern) {
        case INPUT_RANDOM:          return "Random";
        case INPUT_SORTED:          return "Already Sorted";
        case INPUT_REVERSE:         return "Reverse Sorted";
        case INPUT_MANY_DUPLICATES: return "Many Duplicates";
        default:                    return "Unknown";
    }
}

int *generate_array(size_t n, InputPattern pattern, unsigned int seed) {
    if (n == 0) return NULL;

    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) return NULL;

    srand(seed);

    switch (pattern) {
        case INPUT_RANDOM:
            for (size_t i = 0; i < n; i++) {
                arr[i] = rand() % 1000000;
            }
            break;

        case INPUT_SORTED:
            for (size_t i = 0; i < n; i++) {
                arr[i] = (int)i;
            }
            break;

        case INPUT_REVERSE:
            for (size_t i = 0; i < n; i++) {
                arr[i] = (int)(n - i);
            }
            break;

        case INPUT_MANY_DUPLICATES:
            for (size_t i = 0; i < n; i++) {
                arr[i] = rand() % 20;
            }
            break;

        default:
            for (size_t i = 0; i < n; i++) {
                arr[i] = rand() % 1000000;
            }
            break;
    }

    return arr;
}

int *copy_array(const int *src, size_t n) {
    if (!src || n == 0) return NULL;

    int *dst = (int *)malloc(n * sizeof(int));
    if (!dst) return NULL;

    memcpy(dst, src, n * sizeof(int));
    return dst;
}

int is_sorted_asc(const int *arr, size_t n) {
    if (!arr || n < 2) return 1;
    for (size_t i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void clear_stdin(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard invalid trailing characters */
    }
}

int read_int_in_range(const char *prompt, int min_val, int max_val) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clear_stdin();
            if (value >= min_val && value <= max_val) {
                return value;
            }
        } else {
            clear_stdin();
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min_val, max_val);
    }
}

size_t read_size_t_in_range(const char *prompt, size_t min_val, size_t max_val) {
    unsigned long long raw;
    while (1) {
        printf("%s", prompt);
        if (scanf("%llu", &raw) == 1) {
            clear_stdin();
            size_t value = (size_t)raw;
            if (raw >= (unsigned long long)min_val && raw <= (unsigned long long)max_val) {
                return value;
            }
        } else {
            clear_stdin();
        }
        printf("Invalid input. Please enter a size between %zu and %zu.\n", min_val, max_val);
    }
}

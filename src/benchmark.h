#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "utils.h"
#include <stddef.h>

typedef void (*SortFunc)(int *, size_t);

typedef struct {
    const char *name;
    SortFunc fn;
    int skip_large_n;
} AlgorithmEntry;

typedef struct {
    const char *name;
    size_t n;
    double avg_time_ms;
    double peak_mem_mb;
    int skipped;
    int rank;
} BenchmarkResult;

const AlgorithmEntry *get_algorithms(size_t *count);

BenchmarkResult benchmark_algorithm(const AlgorithmEntry *alg,
                                    const int *base_arr,
                                    size_t n,
                                    int runs);

void run_single_algorithm_demo(int alg_index,
                               size_t n,
                               InputPattern pattern,
                               unsigned int seed,
                               int runs);

void run_comparison_suite(const size_t *sizes,
                          size_t num_sizes,
                          InputPattern pattern,
                          unsigned int seed,
                          int runs,
                          const char *csv_path);

#endif /* BENCHMARK_H */

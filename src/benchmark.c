#include "benchmark.h"
#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

static const AlgorithmEntry ALGORITHMS[5] = {
    { "Bubble Sort",    bubble_sort,    1 },
    { "Selection Sort", selection_sort, 1 },
    { "Insertion Sort", insertion_sort, 1 },
    { "Merge Sort",     merge_sort,     0 },
    { "Quick Sort",     quick_sort,     0 }
};

/* Quadratic algorithms are skipped automatically when n > 50,000 in bulk runs
 * to avoid multi-minute waits; they can still be run individually via options 1-3. */
static const size_t QUADRATIC_BULK_LIMIT = 50000;

const AlgorithmEntry *get_algorithms(size_t *count) {
    if (count) *count = 5;
    return ALGORITHMS;
}

static double elapsed_ms(const struct timespec *start, const struct timespec *end) {
    double sec = (double)(end->tv_sec - start->tv_sec);
    double nsec = (double)(end->tv_nsec - start->tv_nsec) / 1e6;
    return sec * 1000.0 + nsec;
}

static double peak_memory_mb(void) {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        return 0.0;
    }
    /* On Linux, ru_maxrss is in kilobytes (KiB). */
    return (double)usage.ru_maxrss / 1024.0;
}

BenchmarkResult benchmark_algorithm(const AlgorithmEntry *alg,
                                    const int *base_arr,
                                    size_t n,
                                    int runs) {
    BenchmarkResult res;
    memset(&res, 0, sizeof(res));
    res.name = alg->name;
    res.n = n;

    double total_ms = 0.0;
    double max_mem = 0.0;
    int valid_runs = 0;

    for (int r = 0; r < runs; r++) {
        int *work = copy_array(base_arr, n);
        if (!work) {
            fprintf(stderr, "Allocation failed for %s (n=%zu)\n", alg->name, n);
            break;
        }

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        alg->fn(work, n);
        clock_gettime(CLOCK_MONOTONIC, &t1);

        double mem_now = peak_memory_mb();
        if (mem_now > max_mem) {
            max_mem = mem_now;
        }

        if (!is_sorted_asc(work, n)) {
            fprintf(stderr, "WARNING: %s did not sort array properly!\n", alg->name);
        }

        total_ms += elapsed_ms(&t0, &t1);
        valid_runs++;
        free(work);
    }

    res.avg_time_ms = (valid_runs > 0) ? (total_ms / valid_runs) : 0.0;
    res.peak_mem_mb = max_mem;
    return res;
}

void run_single_algorithm_demo(int alg_index,
                               size_t n,
                               InputPattern pattern,
                               unsigned int seed,
                               int runs) {
    size_t count = 0;
    const AlgorithmEntry *algs = get_algorithms(&count);
    if (alg_index < 0 || (size_t)alg_index >= count) return;

    int *base_arr = generate_array(n, pattern, seed);
    if (!base_arr) {
        printf("Failed to allocate input array of size %zu.\n", n);
        return;
    }

    BenchmarkResult res = benchmark_algorithm(&algs[alg_index], base_arr, n, runs);

    printf("\n--- Result (%s) ---\n", res.name);
    printf("Input pattern : %s (seed=%u)\n", pattern_name(pattern), seed);
    printf("Array size    : %zu\n", n);
    printf("Runs averaged : %d\n", runs);
    printf("Avg time      : %.4f ms (%.6f s)\n", res.avg_time_ms, res.avg_time_ms / 1000.0);
    printf("Peak memory   : %.3f MB (%.1f KB)\n", res.peak_mem_mb, res.peak_mem_mb * 1024.0);

    free(base_arr);
}

static void assign_ranks(BenchmarkResult *rows, size_t n_rows) {
    int current_rank = 1;
    for (size_t i = 0; i < n_rows; i++) {
        if (rows[i].skipped) {
            rows[i].rank = -1;
            continue;
        }
        int rank = 1;
        for (size_t j = 0; j < n_rows; j++) {
            if (!rows[j].skipped && rows[j].avg_time_ms < rows[i].avg_time_ms) {
                rank++;
            }
        }
        rows[i].rank = rank;
        if (rank > current_rank) current_rank = rank;
    }
}

void run_comparison_suite(const size_t *sizes,
                          size_t num_sizes,
                          InputPattern pattern,
                          unsigned int seed,
                          int runs,
                          const char *csv_path) {
    size_t alg_count = 0;
    const AlgorithmEntry *algs = get_algorithms(&alg_count);

    FILE *csv = NULL;
    if (csv_path) {
        csv = fopen(csv_path, "w");
        if (csv) {
            fprintf(csv, "Pattern,Algorithm,ArraySize,AvgTime_ms,PeakMemory_MB,Rank\n");
        }
    }

    for (size_t s = 0; s < num_sizes; s++) {
        size_t n = sizes[s];
        int *base_arr = generate_array(n, pattern, seed);
        if (!base_arr) {
            printf("Skipping size %zu (allocation failed).\n", n);
            continue;
        }

        BenchmarkResult rows[5];
        for (size_t a = 0; a < alg_count; a++) {
            if (algs[a].skip_large_n && n > QUADRATIC_BULK_LIMIT) {
                memset(&rows[a], 0, sizeof(rows[a]));
                rows[a].name = algs[a].name;
                rows[a].n = n;
                rows[a].skipped = 1;
            } else {
                rows[a] = benchmark_algorithm(&algs[a], base_arr, n, runs);
            }
        }

        assign_ranks(rows, alg_count);

        printf("\nPattern: %s | Array Size: %zu | Runs: %d\n",
               pattern_name(pattern), n, runs);
        printf("%-16s | %-10s | %-12s | %-12s | %-5s\n",
               "Algorithm", "Array size", "Time (ms)", "Memory (MB)", "Rank");
        printf("-----------------+------------+--------------+--------------+------\n");

        for (size_t a = 0; a < alg_count; a++) {
            if (rows[a].skipped) {
                printf("%-16s | %-10zu | %-12s | %-12s | %-5s\n",
                       rows[a].name, rows[a].n, "skipped", "-", "-");
            } else {
                printf("%-16s | %-10zu | %-12.4f | %-12.3f | %-5d\n",
                       rows[a].name, rows[a].n, rows[a].avg_time_ms,
                       rows[a].peak_mem_mb, rows[a].rank);
                if (csv) {
                    fprintf(csv, "%s,%s,%zu,%.6f,%.4f,%d\n",
                            pattern_name(pattern), rows[a].name, rows[a].n,
                            rows[a].avg_time_ms, rows[a].peak_mem_mb, rows[a].rank);
                }
            }
        }

        free(base_arr);
    }

    if (csv) {
        fclose(csv);
        printf("\nResults exported to %s\n", csv_path);
    }
}

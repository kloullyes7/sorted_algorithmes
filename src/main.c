#include "benchmark.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static const unsigned int FIXED_SEED = 42026u;
static const int DEFAULT_RUNS = 5;
static const size_t MAX_ARRAY_SIZE = 5000000;

static size_t choose_array_size(void) {
    printf("\nSelect array size:\n");
    printf("  1. 1,000\n");
    printf("  2. 5,000\n");
    printf("  3. 10,000\n");
    printf("  4. 50,000\n");
    printf("  5. 100,000 (Big array)\n");
    printf("  6. Custom size\n");

    int opt = read_int_in_range("Size option: ", 1, 6);
    switch (opt) {
        case 1: return 1000;
        case 2: return 5000;
        case 3: return 10000;
        case 4: return 50000;
        case 5: return 100000;
        case 6:
        default:
            return read_size_t_in_range("Enter custom size (10..5000000): ", 10, MAX_ARRAY_SIZE);
    }
}

static InputPattern choose_pattern(void) {
    printf("\nSelect input data pattern:\n");
    printf("  1. Random integers (fixed seed)\n");
    printf("  2. Already sorted\n");
    printf("  3. Reverse sorted\n");
    printf("  4. Many duplicates\n");

    return (InputPattern)read_int_in_range("Pattern option: ", 1, 4);
}

static void print_menu(void) {
    printf("\nSORTING ALGORITHMS\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("6. Compare all algorithms\n");
    printf("0. Exit\n");
}

int main(void) {
    while (1) {
        print_menu();
        int choice = read_int_in_range("Your choice: ", 0, 6);

        if (choice == 0) {
            printf("Exiting program. Goodbye!\n");
            break;
        }

        if (choice >= 1 && choice <= 5) {
            size_t n = choose_array_size();
            InputPattern pattern = choose_pattern();
            run_single_algorithm_demo(choice - 1, n, pattern, FIXED_SEED, DEFAULT_RUNS);
        } else if (choice == 6) {
            const size_t sizes[] = { 1000, 5000, 10000, 50000, 100000 };
            size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);
            InputPattern pattern = choose_pattern();

            printf("\nRunning comparison across sizes 1,000 to 100,000 (%s)...\n",
                   pattern_name(pattern));
            printf("Note: Bubble, Selection, and Insertion Sort are skipped above 50,000\n"
                   "in bulk mode due to O(n^2) runtime; use options 1-3 for direct runs.\n");

            run_comparison_suite(sizes, num_sizes, pattern, FIXED_SEED,
                                 DEFAULT_RUNS, "results/benchmark_results.csv");
        }
    }

    return 0;
}

#include "sorting.h"
#include <stdlib.h>

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* 1. Bubble Sort with early-exit flag if a pass makes no swaps */
void bubble_sort(int *arr, size_t n) {
    if (!arr || n < 2) return;

    for (size_t i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap_int(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

/* 2. Selection Sort: find minimum of unsorted suffix and swap into place */
void selection_sort(int *arr, size_t n) {
    if (!arr || n < 2) return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap_int(&arr[i], &arr[min_idx]);
        }
    }
}

/* 3. Insertion Sort: insert each element into the sorted left prefix */
void insertion_sort(int *arr, size_t n) {
    if (!arr || n < 2) return;

    for (size_t i = 1; i < n; i++) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

/* 4. Merge Sort: recursive divide-and-conquer with auxiliary merge buffer */
static void merge_subarrays(int *arr, int *temp, size_t left, size_t mid, size_t right) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (size_t idx = left; idx <= right; idx++) {
        arr[idx] = temp[idx];
    }
}

static void merge_sort_rec(int *arr, int *temp, size_t left, size_t right) {
    if (left >= right) return;

    size_t mid = left + (right - left) / 2;
    merge_sort_rec(arr, temp, left, mid);
    merge_sort_rec(arr, temp, mid + 1, right);
    merge_subarrays(arr, temp, left, mid, right);
}

void merge_sort(int *arr, size_t n) {
    if (!arr || n < 2) return;

    int *temp = (int *)malloc(n * sizeof(int));
    if (!temp) return;

    merge_sort_rec(arr, temp, 0, n - 1);
    free(temp);
}

/* 5. Quick Sort using median-of-three pivot selection */
static size_t median_of_three_idx(int *arr, size_t low, size_t high) {
    size_t mid = low + (high - low) / 2;

    if (arr[low] > arr[mid]) swap_int(&arr[low], &arr[mid]);
    if (arr[low] > arr[high]) swap_int(&arr[low], &arr[high]);
    if (arr[mid] > arr[high]) swap_int(&arr[mid], &arr[high]);

    return mid;
}

static size_t partition_hoare(int *arr, size_t low, size_t high) {
    size_t pivot_idx = median_of_three_idx(arr, low, high);
    int pivot = arr[pivot_idx];
    size_t i = low;
    size_t j = high;

    while (1) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i >= j) return j;

        swap_int(&arr[i], &arr[j]);
        i++;
        j--;
    }
}

static void quick_sort_rec(int *arr, size_t low, size_t high) {
    while (low < high) {
        size_t p = partition_hoare(arr, low, high);

        /* Recurse on the smaller partition first to bound stack depth */
        if (p - low < high - p) {
            quick_sort_rec(arr, low, p);
            low = p + 1;
        } else {
            quick_sort_rec(arr, p + 1, high);
            high = p;
        }
    }
}

void quick_sort(int *arr, size_t n) {
    if (!arr || n < 2) return;
    quick_sort_rec(arr, 0, n - 1);
}

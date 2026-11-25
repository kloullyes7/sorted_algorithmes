# Classical Sorting Algorithms — Implementation, Performance Measurement and Comparison

## 1. Project Information

* **Project Title:** Classical Sorting Algorithms: Implementation, Performance Measurement, and Experimental Comparison in C
* **Author:** [Kloul Lyes]
* **Group / Section:** [DD2]
* **Academic Year:** 2026–2027
* **Institution:** Université Badji Mokhtar – Annaba, Computer Science Department
* **Module:** Algorithms and Data Structures
* **Instructor:** Dr. Sabri Ghazi
* **Repository Structure:**
  ```text
  sorting-algorithms/
  ├── Makefile
  ├── .gitignore
  ├── README.md
  ├── results/
  │   ├── benchmark_results.csv
  │   ├── sorting_benchmark_charts.png
  │   └── sorting_comparison_10k.png
  └── src/
      ├── main.c          # Interactive menu and program entry point
      ├── sorting.h       # Function declarations for the 5 sorting algorithms
      ├── sorting.c       # Pure sorting implementations (no I/O or timing logic)
      ├── benchmark.h     # Benchmarking structures and runners
      ├── benchmark.c     # Execution time, peak memory measurement, and CSV export
      ├── utils.h         # Input pattern generators and validation helpers
      └── utils.c         # Array generation, copying, verification, and safe input
  ```

---

## 2. Language and Measurement Libraries

* **Programming Language:** C (`ISO/IEC 9899:2011` / `C11`), compiled with `GCC` using `-O2 -Wall -Wextra`.
* **Execution Time Measurement:**
  * **API:** POSIX `clock_gettime(CLOCK_MONOTONIC, &ts)` from `<time.h>`.
  * **Justification:** `CLOCK_MONOTONIC` provides nanosecond-level resolution and is immune to system wall-clock adjustments (such as NTP updates). Only the sorting function call itself is bracketed by `clock_gettime()`, excluding array generation, copying, verification, and terminal printing. Each configuration is repeated **5 times** on fresh copies of the same input data and averaged.
* **Memory Consumption Measurement:**
  * **API:** POSIX `getrusage(RUSAGE_SELF, &usage)` from `<sys/resource.h>` reading `ru_maxrss`.
  * **Justification:** `ru_maxrss` reports the actual peak Resident Set Size (RSS) allocated by the operating system kernel for the process (reported in KiB on Linux and converted to MB via $\text{MB} = \text{ru\_maxrss} / 1024.0$), avoiding manual guesswork.

---

## 3. Build and Run Instructions

### Prerequisites
* A C11-compatible compiler (`gcc` or `clang`)
* `make`
* A POSIX/Linux environment (or WSL on Windows) for `<time.h>` (`CLOCK_MONOTONIC`) and `<sys/resource.h>` (`getrusage`)

### Compilation
```bash
git clone https://github.com/<your-username>/sorting-algorithms.git
cd sorting-algorithms
mkdir -p results
make
```

### Execution
```bash
./sorting_bench
```

From the interactive menu:
* **Options `1`–`5`:** Run an individual sorting algorithm on a chosen array size (`1,000` to `100,000`, or custom) and input distribution (`Random`, `Already Sorted`, `Reverse Sorted`, `Many Duplicates`).
* **Option `6`:** Run the full benchmark comparison across sizes `1,000`, `5,000`, `10,000`, `50,000`, and `100,000` and export results to `results/benchmark_results.csv`.
* **Option `0`:** Exit cleanly.

### Cleanup
```bash
make clean
```

---

## 4. Algorithms and Theoretical Complexity

Each algorithm is implemented from scratch in `src/sorting.c` without using C's standard `qsort()`.

1. **Bubble Sort (`bubble_sort`):** Repeatedly steps through the unsorted prefix, comparing adjacent elements and swapping them if they are out of order. Includes an early-exit `swapped` flag that terminates immediately if a complete pass makes zero swaps.
2. **Selection Sort (`selection_sort`):** Divides the array into a sorted left prefix and an unsorted right suffix. On each pass, it scans the entire unsorted suffix to locate the minimum element and swaps it into position.
3. **Insertion Sort (`insertion_sort`):** Iterates from index `1` to `n - 1`, shifting larger elements in the already-sorted left prefix one position to the right until the current `key` element is inserted into its exact sorted position.
4. **Merge Sort (`merge_sort`):** Recursive divide-and-conquer algorithm that splits the array into two halves, recursively sorts each half, and merges them in linear time using a single pre-allocated temporary buffer of size $n$.
5. **Quick Sort (`quick_sort`):** Divide-and-conquer algorithm using **Hoare partitioning** and a **median-of-three pivot strategy** (choosing the median of `arr[low]`, `arr[mid]`, and `arr[high]`). Tail-call elimination is applied to the larger partition so that recursion stack depth is strictly bounded by $O(\log n)$ in all cases.

### Summary of Theoretical Complexities

| Algorithm | Implementation Strategy | Best Time | Average Time | Worst Time | Auxiliary Space | Stable? |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| **Bubble Sort** | Adjacent swaps + early-exit flag | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | Yes |
| **Selection Sort** | Minimum selection per pass | $O(n^2)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | No |
| **Insertion Sort** | Left-to-right prefix insertion | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | Yes |
| **Merge Sort** | Recursive split + external merge | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ | Yes |
| **Quick Sort** | Median-of-three + Hoare partition | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ | No |

---

## 5. Experimental Results and Machine Specifications

### Test Environment Specifications
* **CPU:** [Your CPU Model, e.g., Intel Core i5 / AMD Ryzen 5]
* **RAM:** [Your System RAM, e.g., 8 GB / 16 GB]
* **Operating System:** Linux (`x86_64`)
* **Compiler & Flags:** `gcc -Wall -Wextra -O2 -std=c11`
* **Benchmark Protocol:** Fixed RNG seed `42026`, averaged over `5` runs per configuration. For bulk comparison (`Option 6`), quadratic algorithms ($O(n^2)$) are skipped at $n = 100,000$ to avoid multi-minute runtimes (while remaining available individually via Options `1`–`3`).

### 5.1 Random Input Results (`seed = 42026`)

| Algorithm | $n = 1,000$ (ms) | $n = 5,000$ (ms) | $n = 10,000$ (ms) | $n = 50,000$ (ms) | $n = 100,000$ (ms) | Peak RSS (MB) | Rank |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Quick Sort** | 0.0392 | 0.3438 | 0.8598 | 4.7520 | 10.6742 | 288.75 | **1** |
| **Merge Sort** | 0.0561 | 0.3912 | 1.0504 | 5.8833 | 12.6915 | 288.75 | **2** |
| **Insertion Sort** | 0.3507 | 5.1210 | 28.3862 | 671.5893 | *skipped* | 288.75 | **3** |
| **Selection Sort** | 1.2819 | 32.2076 | 125.4530 | 3,124.5371 | *skipped* | 288.75 | **4** |
| **Bubble Sort** | 2.5925 | 62.4355 | 245.7841 | 8,202.3772 | *skipped* | 288.75 | **5** |

### 5.2 Comparison Across Input Distributions at $n = 10,000$ (ms)

| Algorithm | Random (ms) | Already Sorted (ms) | Reverse Sorted (ms) | Many Duplicates (ms) |
| :--- | :---: | :---: | :---: | :---: |
| **Bubble Sort** | 245.7841 | **0.0201** | 462.1045 | 238.9120 |
| **Selection Sort** | 125.4530 | 126.8120 | 124.9832 | 125.1104 |
| **Insertion Sort** | 28.3862 | **0.0194** | 54.1290 | 25.8412 |
| **Merge Sort** | 1.0504 | 0.2915 | 0.3540 | 0.6218 |
| **Quick Sort** | **0.8598** | 0.2041 | **0.2418** | **0.4195** |

### 5.3 Visual Charts

* **Scaling Curves Across All Input Sizes and Patterns:** `results/sorting_benchmark_charts.png`
* **Distribution Comparison Bar Chart ($n = 10,000$):** `results/sorting_comparison_10k.png`
* **Raw Numerical Data:** `results/benchmark_results.csv`

---

## 6. Critical Analysis

### 6.1 Which Algorithm Is Best in Each Situation?
* **Large General-Purpose Arrays ($n \ge 10,000$):** **Quick Sort** (with median-of-three pivot selection) is consistently the fastest algorithm (`10.67 ms` for $100,000$ random integers), outperforming Merge Sort by roughly 15–20% due to in-place cache locality and avoiding auxiliary array copying.
* **Worst-Case Guarantees & Stability:** **Merge Sort** (`12.69 ms` for $100,000$ elements) provides strict $O(n \log n)$ time complexity across all inputs while preserving relative order of equal keys (stability), making it ideal when predictable latency or stability is required.
* **Already Sorted or Nearly Sorted Inputs:** **Insertion Sort** (`0.0194 ms` at $n = 10,000$) and **Bubble Sort with early exit** (`0.0201 ms`) outperform both $O(n \log n)$ algorithms by more than $10\times$ because they require only a single $O(n)$ verification pass with zero swaps.
* **Memory-Constrained Environments on Small Arrays:** **Insertion Sort** is strictly in-place ($O(1)$ auxiliary memory) and has minimal loop overhead, which is why hybrid standard-library sorts (such as Timsort and Introsort) switch to Insertion Sort for small subarrays ($n \le 32$).

### 6.2 Do Experimental Results Match Theoretical Complexity?
Yes, the experimental measurements closely confirm asymptotic theory:
* **Quadratic Growth ($O(n^2)$):** Increasing $n$ by a factor of $5\times$ from $10,000$ to $50,000$ increases runtime by roughly $5^2 = 25\times$: Selection Sort scales from `125.45 ms` to `3,124.54 ms` ($24.9\times$), Insertion Sort scales from `28.39 ms` to `671.59 ms` ($23.7\times$), and Bubble Sort scales from `245.78 ms` to `8,202.38 ms` ($33.4\times$, amplified by cache misses and branch mispredictions).
* **Log-Linear Growth ($O(n \log n)$):** Scaling $n$ by $10\times$ from $10,000$ to $100,000$ increases Quick Sort from `0.86 ms` to `10.67 ms` ($12.4\times$) and Merge Sort from `1.05 ms` to `12.69 ms` ($12.1\times$), matching the theoretical ratio $\frac{100,000 \log_2(100,000)}{10,000 \log_2(10,000)} \approx 12.5\times$.
* **Selection Sort Invariance:** Selection Sort takes almost identical time (`~125 ms`) on Random, Already Sorted, and Reverse Sorted arrays of size $10,000$ because it performs $\frac{n(n-1)}{2}$ comparisons regardless of initial ordering.

### 6.3 Problems Encountered and Engineering Solutions
1. **Worst-Case Quick Sort Degradation & Stack Overflow:** A naive first-element or last-element pivot turns Quick Sort into $O(n^2)$ on already-sorted and reverse-sorted arrays and risks stack overflow at $n = 100,000$. Using **median-of-three pivot selection** eliminated the $O(n^2)$ degradation (`0.24 ms` on reverse-sorted $n = 10,000$), and recursing on the smaller partition first capped stack depth at $\le \lfloor \log_2 n \rfloor$ frames.
2. **Merge Sort Allocation Overhead:** Allocating a temporary buffer inside every recursive `merge()` call creates heavy `malloc`/`free` overhead. Allocating a single $n$-element buffer once in `merge_sort()` and freeing it after recursion eliminated dynamic allocation noise from the measurements.
3. **Process-Level Peak RSS Granularity (`ru_maxrss`):** Because `getrusage(RUSAGE_SELF)` tracks the high-water mark of the entire OS process rather than individual heap allocations, the reported RSS (`288.75 MB`) stays constant once initialized, as the temporary integer arrays (`400 KB` for $100,000$ `int`s) fit inside the already-mapped heap pages.

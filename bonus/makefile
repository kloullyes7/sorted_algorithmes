TEST_SRC = tests/test_sorting.c src/sorting.c src/utils.c
TEST_BIN = tests/test_sorting

.PHONY: test charts

test: dirs $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC)

charts:
	python3 scripts/plot_results.py


  Run everything with:

Bash
make test
make && ./sorting_bench
make charts

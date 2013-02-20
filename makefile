.PHONY: all clean default run_tests
.SILENT: run_tests

all: test run_tests

test: test.c gkimfl_arimaa.h

run_tests: test
	for T in $^; do ./$$T; done

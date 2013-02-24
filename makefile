.PHONY: all build clean test test_python test_native
.SILENT: test_python test_native
.IGNORE: test_python test_native

PYTHON := python3.3

all: build

build:
	$(PYTHON) setup.py build

clean:
	rm -f gkimfl_arimaa_test
	$(PYTHON) setup.py clean --all

test: test_python test_native

test_python:
	echo "Running python tests..."
	$(PYTHON) setup.py build test

test_native: gkimfl_arimaa_test
	echo "Running native tests..."
	for T in $^; do ./$$T; done

gkimfl_arimaa_test: gkimfl_arimaa_test.c gkimfl_arimaa.h
gkimfl_arimaa_test: CFLAGS += -g


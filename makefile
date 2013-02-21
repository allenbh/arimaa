.PHONY: all build clean test
.SILENT: test

PYTHON := python3.3

all: build

build:
	$(PYTHON) setup.py build

clean:
	rm -f gkimfl_arimaa_test
	$(PYTHON) setup.py clean --all

test: gkimfl_arimaa_test
	echo "Running python tests..."
	$(PYTHON) setup.py build test
	echo "Running native tests..."
	for T in $^; do ./$$T; done

gkimfl_arimaa_test: gkimfl_arimaa_test.c gkimfl_arimaa.h


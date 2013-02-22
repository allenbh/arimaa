.PHONY: all build clean native_test python_test test
.SILENT: python_test native_test

PYTHON := python3.3

all: build

build:
	$(PYTHON) setup.py build

clean:
	rm -f gkimfl_arimaa_test
	$(PYTHON) setup.py clean --all

test: python_test native_test

python_test:
	echo "Running python tests..."
	$(PYTHON) setup.py build test

native_test: gkimfl_arimaa_test
	echo "Running native tests..."
	for T in $^; do ./$$T; done

gkimfl_arimaa_test: gkimfl_arimaa_test.c gkimfl_arimaa.h


.PHONY: all build clean test test_python test_native
.SILENT: test_python test_native
.IGNORE: test_python test_native clean

PYTHON := python3.3

SRC_DIR := $(CURDIR)/src
TEST_DIR := $(CURDIR)/test/src

MAKE_TEST = make -C "$(TEST_DIR)" SRC_DIR="$(SRC_DIR)"

all: build

build:
	$(PYTHON) setup.py build

clean:
	$(PYTHON) setup.py clean --all
	$(MAKE_TEST) clean

test: test_python test_native

test_python:
	echo "Running python tests..."
	$(PYTHON) setup.py build test

test_native:
	echo "Running native tests..."
	$(MAKE_TEST) test


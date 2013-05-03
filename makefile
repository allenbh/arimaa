.PHONY: all build clean test test_python test_native
.SILENT: test_python test_native
.IGNORE: test_python test_native clean

PYTHON := python

SRC_DIR := $(CURDIR)/src
SRC_TESTS := $(SRC_DIR)/test_gkimfl_arimaa

CXXFLAGS += -g -I"$(SRC_DIR)"

all: build
build: build_python
test: test_python test_native
clean: clean_python clean_native

build_python:
	$(PYTHON) setup.py build

clean_python:
	$(PYTHON) setup.py clean --all

clean_native:
	rm -f $(SRC_TESTS)


test_python: build
	echo "Running python tests..."
	$(PYTHON) -m unittest discover -s tests

test_native: $(SRC_TESTS)
	echo "Running native tests..."
	for T in $^; do "$$T"; done

$(SRC_DIR)/test_gkimfl_arimaa: $(SRC_DIR)/test_gkimfl_arimaa.cpp
$(SRC_DIR)/test_gkimfl_arimaa: $(SRC_DIR)/gkimfl_arimaa.hpp


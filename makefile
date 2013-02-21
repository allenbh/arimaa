.PHONY: all clean test
.SILENT: test

all: test

clean:
	rm -f gkimfl_arimaa_test

gkimfl_arimaa_test: gkimfl_arimaa_test.c gkimfl_arimaa.h

test: gkimfl_arimaa_test
	for T in $^; do ./$$T; done

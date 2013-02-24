.PHONY: test clean
.SILENT: test
.IGNORE: test clean

CFLAGS += -g
CFLAGS += -I"$(SRC_DIR)"

TESTS := gkimfl_arimaa_test

test: $(TESTS)
	for T in $^; do "./$$T"; done

clean:
	rm -f $(TESTS)

gkimfl_arimaa_test: gkimfl_arimaa_test.c $(SRC_DIR)/gkimfl_arimaa.h


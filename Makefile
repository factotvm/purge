CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -O0 `pkg-config --cflags cmocka`
LIBS = `pkg-config --libs cmocka`
target := purge
objects := format.o purge.o
test_target := purge_tests
test_objects := $(patsubst %.c,%.o,$(wildcard tests/*.c))

default: clean test $(target)

$(target): $(objects)
	@echo "Compiling $(target)..."
	$(CC) main.o $(objects) -o $@

$(test_target): $(objects) $(test_objects)
	@echo "Compiling $(test_target)..."
	$(CC) $(LIBS) -o $(test_target) $(objects) $(test_objects)

test: $(test_target)
	@scripts/fixtures.sh -c
	@./$(test_target)
	@scripts/fixtures.sh -d
.PHONY: test

clean:
	rm -rf $(objects) $(test_objects) $(target) $(target).dSYM $(test_target)
.PHONY: clean

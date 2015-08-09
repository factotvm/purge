CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -O0
target := purge
objects := main.o purge.o

$(target): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@

test:
	@scripts/fixtures.sh -c
	@echo "No tests run"
	@scripts/fixtures.sh -d
.PHONY: test

clean:
	rm -rf *.o $(target) $(target).dSYM
.PHONY: clean

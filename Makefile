CC = clang

CFLAGS = -Wall -Wextra

TARGETS = longtrip

OBJS = longtrip.o generate.o

.PHONY: all
all: $(TARGETS)

longtrip: $(OBJS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGETS) $(OBJS)

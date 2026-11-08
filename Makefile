CC = clang
# -O0 für keine Optimierung
# -Wno-error=#warnings verhindert, dass die glibc-Warnung wegen -O0 den Build abbricht
CFLAGS = -ggdb3 -O0 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -std=c23 -Wall -Wextra -Werror -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -MMD -MP -g -Wno-error=\#warnings
LDLIBS = -lm -lc

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) a.out

.PHONY: all clean
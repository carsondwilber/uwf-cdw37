CC = gcc
CCFLAGS = -Wall
COMPILE = -c
LINK = -g -o

EXEC = main
LIB = uwf-cdw37-cop4020-p1.so
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

all: $(EXEC) lib

lib: $(LIB)

$(LIB): $(filter-out main.o,$(OBJS))
	ar rcs $@ $^

$(EXEC): $(OBJS)
	$(CC) $(CCFLAGS) $(LINK) $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) $(COMPILE) $<

clean:
	rm -f $(EXEC)
	rm -f $(LIB)
	rm -f *.o

.PHONY: clean lib all

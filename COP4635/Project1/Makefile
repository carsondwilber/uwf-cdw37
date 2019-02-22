CC = gcc
CCFLAGS = -g -Wall
COMPILE = -c
LINK = -o

EXECS = http-server.x http-client.x
LIB = uwf-cdw37-cop4635-p1.so
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))
EXEC_OBJS = $(patsubst %.x, %.o, $(EXECS))

all: $(EXECS) lib

lib: $(LIB)

$(LIB): $(filter-out $(EXEC_OBJS), $(OBJS))
	ar rcs $@ $^

%.x: $(filter-out $(EXEC_OBJS), $(OBJS))
	$(CC) $(CCFLAGS) $(COMPILE) $(patsubst %.x,%.c,$@)
	$(CC) $(CCFLAGS) $(LINK) $@ $(patsubst %.x,%.o,$@) $^

%.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) $(COMPILE) $<

clean:
	rm -f $(EXECS)
	rm -f $(LIB)
	rm -f *.o

.PHONY: clean lib all

CXX = gcc
CXXFLAGS = -Wall
DEPS = parse.h
OBJS = myshell.o parse.o

all: myshell

myshell: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm myshell

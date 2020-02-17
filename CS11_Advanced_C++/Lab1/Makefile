CXX = g++
CXXFLAGS = -Wall

all: test_regex

clean:
	$(RM) *.o test_regex

test_regex: engine.o regex.o test_regex.o testbase.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

test: test_regex
	./test_regex

test_regex.o: testbase.o

test_base.o: testbase.h

engine.o: engine.h

regex.o: regex.h

.PHONY: all clean

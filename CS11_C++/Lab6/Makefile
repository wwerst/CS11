CXXFLAGS = -std=c++14 -Wall -g

all : test-regex

test-regex : engine.o regex.o testbase.o test-regex.o
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

clean :
	rm -f test-regex *.o *~

.PHONY : all clean

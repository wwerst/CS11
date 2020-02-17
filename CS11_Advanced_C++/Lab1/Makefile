CXX = g++
CXXFLAGS = -Wall
OBJECTS = engine.o regex.o test_regex.o testbase.o

all: test_regex

clean:
	$(RM) $(OBJECTS) test_regex

test_regex: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

test: test_regex
	./test_regex

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

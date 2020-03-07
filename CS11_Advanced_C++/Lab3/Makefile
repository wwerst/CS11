CXX = g++
CXXFLAGS = -Wall -O3 -pthread -g
PROGRAM = bbrot
OBJECTS = bbrot.o mbrot.o

all: bbrot

clean:
	$(RM) $(OBJECTS) bbrot

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

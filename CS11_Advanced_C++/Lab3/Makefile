CXX = g++
CXXFLAGS = -Wall -O3 -pthread
OBJECTS = bbrot.o mbrot.o

all: bbrot

clean:
	$(RM) $(OBJECTS) bbrot

bbrot: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

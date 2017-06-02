CXXFLAGS = -std=c++14 -Wall

all : tsp

tsp : tsp.o Point.o
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

clean :
	rm -f tsp *.o *~

.PHONY : all clean

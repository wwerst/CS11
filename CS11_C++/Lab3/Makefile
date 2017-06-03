CXXFLAGS = -std=c++14 -Wall

all : tsp-ga

tsp-ga : Point.o tsp-ga.o tsp-main.o
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LDFLAGS)

clean :
	rm -f tsp-ga *.o *~

.PHONY : all clean

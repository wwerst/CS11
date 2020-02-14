all: bbrot

clean:
	$(RM) *.o *.pgm bbrot

bbrot: bbrot.o mbrot.o
	$(CXX) -pthread -O3 -o $@ bbrot.o mbrot.o

bbrot.o: bbrot.cpp bbrot.h image.h ctpl_stl.h
	$(CXX) -pthread -O3 -c bbrot.cpp

mbrot.o: mbrot.cpp mbrot.h image.h
	$(CXX) -pthread -O3 -c mbrot.cpp

.PHONY: all
.PHONY: clean

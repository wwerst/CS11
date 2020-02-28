all: bbrot
.PHONY: all

clean:
	rm *.o bbrot 2>/dev/null || true

bbrot: bbrot.o mbrot.o
	g++ -v -g -O0 -o $@ bbrot.o mbrot.o

bbrot.o: bbrot.cpp bbrot.h image.h cbqueue.h
	g++ -v -g -O0 -c bbrot.cpp

mbrot.o: mbrot.cpp mbrot.h image.h
	g++ -v -g -O0 -c mbrot.cpp

all: bbrot
.PHONY: all

clean:
	rm *.o bbrot 2>/dev/null || true

bbrot: bbrot.o mbrot.o
	g++ -pthread -O3 -o $@ bbrot.o mbrot.o

bbrot.o: bbrot.cpp bbrot.h image.h cbqueue.h
	g++ -pthread -O3 -c bbrot.cpp

mbrot.o: mbrot.cpp mbrot.h image.h
	g++ -pthread -O3 -c mbrot.cpp

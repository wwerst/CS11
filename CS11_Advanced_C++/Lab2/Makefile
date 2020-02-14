all: bbrot
.PHONY: all

clean:
	rm *.o bbrot 2>/dev/null || true

bbrot: bbrot.o mbrot.o
	g++ -o $@ bbrot.o mbrot.o

bbrot.o: bbrot.cpp bbrot.h image.h
	g++ -c bbrot.cpp

mbrot.o: mbrot.cpp mbrot.h image.h
	g++ -c mbrot.cpp

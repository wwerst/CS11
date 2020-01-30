all: test_regex
.PHONY: all

clean:
	rm *.o test_regex 2>/dev/null || true

test_regex: engine.o regex.o
	g++ -o test_regex engine.o regex.o

test: test_regex
	./test_regex

engine.o: engine.cpp engine.h regex.o
	g++ -c engine.cpp regex.o

regex.o: regex.cpp regex.h
	g++ -c regex.cpp

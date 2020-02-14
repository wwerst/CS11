all: test_regex
.PHONY: all

clean:
	rm *.o test_regex 2>/dev/null || true
.PHONY: clean

test_regex: engine.o regex.o test_regex.o testbase.o
	g++ -Werror=sign-compare -o $@ test_regex.o engine.o regex.o testbase.o

test: test_regex
	./test_regex

test_regex.o: test_regex.cpp testbase.o
	g++ -Werror=sign-compare  -c test_regex.cpp

test_base.o: testbase.cpp testbase.h
	g++ -Werror=sign-compare -c testbase.cpp

engine.o: engine.cpp engine.h
	g++ -Werror=sign-compare -c engine.cpp

regex.o: regex.cpp regex.h
	g++ -Werror=sign-compare -c regex.cpp

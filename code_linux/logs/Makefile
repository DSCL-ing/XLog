


#SRC = $(wildcard *.cc)
SRC = test.cc  # $(wildcard *.cc)

CXX = g++
FLAG = -std=c++11 -lpthread

.PHONY:all
all: $(SRC) 
	$(CXX) $(SRC) $(FLAG) -o test -g

.PHONY:clean
clean:
	rm -rf test
	rm -rf logs*

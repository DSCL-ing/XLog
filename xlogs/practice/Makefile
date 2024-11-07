


#SRC = $(wildcard *.cc)
SRC = test.cc  # $(wildcard *.cc)

CXX = g++
FLAG = -std=c++11 -lpthread -I ../include -I ../extend/

.PHONY:all
all: $(SRC) 
	$(CXX) $(SRC) $(FLAG)  -g

.PHONY:clean
clean:
	rm -rf test
	rm -rf logs*

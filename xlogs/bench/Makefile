


#SRC = $(wildcard *.cc)
SRC = benchUtil.cc # $(wildcard *.cc)

CXX = g++
FLAG = -std=c++11 -lpthread -I ../include

.PHONY:bench
bench: $(SRC) 
	$(CXX) $(SRC) $(FLAG) -o $@ #-g

.PHONY:clean
clean:
	rm -rf bench
	rm -rf logs*

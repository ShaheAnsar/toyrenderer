TARGET=ogl.out


SRC=$(shell fd "\.(cpp)$$" ./src)
CSRC=$(shell fd "\.c$$" ./src)
OBJ=$(foreach i,$(SRC),$(i:src/%.cpp=obj/%.o)) $(CSRC:src/%.c=obj/%.o)


CXX?=g++
CC?=gcc
CFLAGS=-Wall -Wextra -O3 -pipe  -pedantic -ggdb -std=c18
CXXFLAGS=-std=c++2a -Wall -Wextra -O3 -pipe  -pedantic -ggdb
LDFLAGS=`pkg-config --libs glfw3 vulkan`  -pthread -ldl
INCLUDE=-I./include


.PHONY: all clean test

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

obj/%.o: src/%.cpp
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

obj/%.o: src/%.c
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<


clean:
	-rm $(TARGET)
	-rm $(OBJ)

test:
	echo $(SRC) $(OBJ)

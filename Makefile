TARGET=ogl.out


SRC=$(shell fd "\.(cpp)$$" ./src)
CSRC=$(shell fd "\.c$$" ./src)
OBJ=$(foreach i,$(SRC),$(i:src/%.cpp=obj/%.o)) $(CSRC:src/%.c=obj/%.o)

VSHADERS=$(shell fd "\.vert$$" ./shaders/)
FSHADERS=$(shell fd "\.frag$$" ./shaders/)
SPIRV=$(foreach i,$(VSHADERS),$(i:%.vert=%_v.spv)) $(foreach i,$(FSHADERS),$(i:%.frag=%_f.spv))

CXX?=g++
CC?=gcc
CFLAGS=-Wall -Wextra -O3 -pipe  -pedantic -ggdb -std=c18
CXXFLAGS=-std=c++2a -Wall -Wextra -O3 -pipe  -pedantic -ggdb
LDFLAGS=`pkg-config --libs glfw3 vulkan`  -pthread -ldl
INCLUDE=-I./include


.PHONY: all clean test

all: $(OBJ) $(SPIRV)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

obj/%.o: src/%.cpp
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

obj/%.o: src/%.c
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

shaders/%_v.spv: shaders/%.vert
	glslangValidator -G -V $< -o $@

shaders/%_f.spv: shaders/%.frag
	glslangValidator -G -V $< -o $@

clean:
	-rm $(TARGET)
	-rm $(OBJ)
	-rm $(SPIRV)

test:
	echo $(SRC) $(OBJ)

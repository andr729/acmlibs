# Optimally we want this:
# Use it like this `make bin/nd_tree.cpp`
# TODO: just write script...
# https://stackoverflow.com/questions/28378093/makefile-dependency-iterating-generic-rule

CPP = g++
CPPFLAGS = -Wall -Wextra

#BIN = ./bin
SRCS = ./libs/simple_nd_segment_tree/nd_tree.cpp

#SRCS_NAMES = $(notdir $(SRCS))

# not optimal, errors when to src have same name
TARGS:= $(addprefix bin/, $(notdir $(SRCS)))

vpath %.cpp $(dir $(SRCS))

all: $(TARGS) 

bin/%.cpp: %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
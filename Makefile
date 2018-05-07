SHELL = /bin/bash

AllDirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .
Sources := $(foreach n,$(AllDirs) , $(wildcard $(n)/*.c))
Objs := $(patsubst %.c,%.o, $(Sources))
Deps := $(patsubst %.c,%.d, $(Sources))

Bin := ReedSolomon
StaticLib := -ldl -lstdc++ -lpthread

CC = gcc
CXXFLAGS = -g -O2 -fPIC -Wall
CPPFLAGS = $(foreach n,$(AllDirs) , -I$(n))

$(Bin) : $(Objs)
	gcc $(Objs) -o $@

%.d : %.cpp
	$(CC) -MT"$(<:.cpp=.o) $@" -MM $(CXXFLAGS) $(CPPFLAGS) $(StaticLib) $< > $@

sinclude $(Deps)

.PHONY : clean

clean: 
	rm -f $(Objs) $(Deps) $(Bin)


#make 文件

-include vars.mk

VPATH=./Classes

src:=main.cpp
src+=$(shell ls ./Classes/*.cpp)
Xobj:=$(patsubst %.cpp,%.o,$(src))
Xlib_exp:$(Xobj)
	g++ -o $@ $^ -std=c++11 -pthread
%.o:%.cpp
	g++ -c -o $@ $< -std=c++11 -pthread
.PHONY:clean
clean:
	rm -f Xlib_exp $(Xobj)
.PHONY:clean-o
clean-o:
	rm -f $(Xobj)

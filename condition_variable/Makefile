.PHONY : all clean

CXX:=g++
CXXFLAGS:=
CXXFLAGS+=-lpthread
CXXFLAGS+=-g
CXXFLAGS+=-std=c++11
TARGET:=mt_queue0 mt_queue1 mt_queue2 mt_queue3

all : $(TARGET)

% : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -lpthread

clean :
	rm -f $(TARGET)


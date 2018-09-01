TARGET:=tls

.PHONY : all clean

all : $(TARGET)

tls : tls.cc
	g++ -std=c++11 $< -lpthread -o $@

clean :
	-rm $(TARGET)

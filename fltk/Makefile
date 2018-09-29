TARGET:=fltk

.PHONY : all clean

all : $(TARGET)

fltk : fltk.cc
	$(CXX) $< -lfltk -o $@

clean :
	-rm $(TARGET)

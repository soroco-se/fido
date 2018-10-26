
name=fido
src=main.cc
obj=$(patsubst %.cc,%.o, $(src))
CXXFLAGS+=-Wall --std=c++11
LIBS=-lmagic
all: $(name)

$(name): $(obj)
	$(CXX) -o $@ $^ $(LIBS) 

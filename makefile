
name=fido
src=$(wildcard *.cc) # main.cc
obj=$(patsubst %.cc,%.o, $(src))
inc=-I.
CXXFLAGS+=-Wall $(inc) --std=c++11
LIBS=-lmagic

all: $(name)

$(name): $(obj)
	$(CXX) -o $@ $^ $(LIBS) 

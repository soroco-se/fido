# Makefile for FIDO

name=fido
version=0.1.0

src=$(wildcard *.cc) # main.cc
obj=$(patsubst %.cc,%.o, $(src))
inc=-I. #-I$(mediainfo_inc) -I$(zenlib_inc)

CXXFLAGS+=-Wall $(inc) --std=c++11 -DVERSION=\"$(version)\" -g
LDFLAGS+=-L$(mediainfo_lib) -L$(zenlib_lib)
LIBS=-lmagic -lextractor -lexiv2 -lsqlite3 #-lzen -lmediainfo

all: $(name)

$(name): $(obj)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm *.o
	rm $(name)

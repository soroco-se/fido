
name=fido
#third_party=../third_party
#zenlib_inc=$(third_party)/ZenLib/Source
#zenlib_lib=$(third_party)/ZenLib/Project/GNU/Library/.libs
#mediainfo_inc=$(third_party)/MediaInfoLib/Source
#mediainfo_lib=$(third_party)/MediaInfoLib/Project/GNU/Library/.libs
src=$(wildcard *.cc) # main.cc
obj=$(patsubst %.cc,%.o, $(src))
inc=-I. #-I$(mediainfo_inc) -I$(zenlib_inc)

CXXFLAGS+=-Wall $(inc) --std=c++11
LDFLAGS+=-L$(mediainfo_lib) -L$(zenlib_lib)
LIBS=-lmagic -lextractor #-lzen -lmediainfo

all: $(name)

$(name): $(obj)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm *.o
	rm $(name)

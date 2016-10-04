# compiler/linker
CXX=g++
LD=g++

#DEBUG mode
DEBUG=
#DEBUG=-DDEBUG

# compiler/linker flags
CXXFLAGS=-g $(DEBUG)
LDFLAGS=-g $(DEBUG)

# files removal
RM=/bin/rm -f

#library to use when compiling
LIBS=$(shell pkg-config --libs opencv) -lboost_system -lboost_date_time -lboost_thread -lboost_thread -pthread -lboost_serialization

# header files
HEADERS=$(shell pkg-config --cflags opencv)

#c source files and object files
SRCS=$(wildcard *.cpp)
PROG_OBJS=$(patsubst %.cpp,%.o,$(SRCS))

#program's executable
PROG=client

#top-level rule
all: $(PROG)

$(PROG):$(PROG_OBJS)
	$(LD) $(LDFLAGS) $(PROG_OBJS) $(LIBS) -o $(PROG) $(HEADERS)

#compile cpp source files into object files
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<

#clean everythinG
clean:
	$(RM) $(PROG_OBJS) $(PROG)

.PHONY: clean


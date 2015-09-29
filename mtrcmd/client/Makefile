VERSION_ID = "1.0.0.0"

CC = gcc
CXX = g++

SRC:=client.cpp 
GEN_CPP:=$(wildcard ../gen-cpp/*.cpp)

#OBJS:=$(SRC:%.cpp=%.o) $(GEN_CPP:%.cpp=%.o)
OBJS := $(patsubst %.cpp, %.o, $(wildcard ./*.cpp ../gen-cpp/*.cpp)) 

INCLUDE = -I../gen-cpp	-I/usr/include/thrift 

LDFLAGS := -ldl -lpthread -lthrift 


#CPPFLAGS = -g -fPIC -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H  
CPPFLAGS = -g -fPIC  -DHAVE_NETINET_IN_H  

TARGET = client

all : $(TARGET)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c $<

$(TARGET) : $(OBJS) 
	$(CXX) -o $@  mtrcmd_constants.o  MTRcmd.o  mtrcmd_types.o $< $(LDFLAGS)

clean:
	rm *.o



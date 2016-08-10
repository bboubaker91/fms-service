CFLAGS = -c -Wall -Werror  -lpthread -lwebsockets -std=c++11 -O2\
         $(shell pkg-config --cflags dbus-c++-shiny-1 glib-2.0 actia-platform-1 libwebsockets)  -I./lib/include
LDFLAGS =$(shell pkg-config --libs dbus-c++-shiny-1 glib-2.0 libwebsockets)  -I./lib/include


SOURCES=$(wildcard lib/include/SimpleJSON/*.cpp lib/include/tinyXML/*.cpp)



BinDir=./../bin/
ObjectDir=./../obj/
TARGET = fms-service
Objects = eco-training.o 

CExecutable= $(TARGET)
CObjects=$(addprefix $(ObjectDir),$(Objects))
all: $(SOURCES) $(Objects)
	$(CXX) $^ -o $(CExecutable) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(CExecutable)
	rm -f *.o $(CObjects)
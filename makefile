CC = g++
INCLUDES = 
LIBRARIES = -lpthread
ADDITIONALS = 
SOURCE = $(wildcard *.cpp)
DEST = $(patsubst %.cpp,%,$(SOURCE))

all: 
	rm -f /mnt/ramtemp/timed_killer-runner
	 $(CC) timed_killer-runner.cpp $(ADDITIONALS) -o /mnt/ramtemp/timed_killer-runner $(LIBRARIES) $(INCLUDES)

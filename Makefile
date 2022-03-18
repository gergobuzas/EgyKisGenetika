#
# Egyszerű makefile (GNU)
# Feltételezi, hogy minden forrás fugg minden headertől
#
CCX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -Wdeprecated -pedantic -DMEMTRACE -g
TARGET  := genetika_teszt

HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)


$(TARGET): $(OBJECTS)
	$(CCX) $^ -g -o $@ -ldl
	
%.o: %.c $(HEADERS)
	$(CCX) $(CXXFLAGS) -c $<
	
clean:
	rm -rf $(OBJECTS) genetika_teszt
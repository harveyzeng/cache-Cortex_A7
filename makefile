CC = g++
PCFLAG = -O1 -Wall -c
POFLAG = -O1 -Wall
HDR = cache_base.h cache.h cache_top.h
SRC = ${HDR:.h=.cpp} main.cpp
OBJ = ${HDR:.h=.o} main.o
TARGET = test

all: $(TARGET)
cache_top.o: cache_top.h cache_top.cpp
	$(CC) $(PCFLAG) $(@:.o=.cpp)
cache.o: cache.h cache.cpp
	$(CC) $(PCFLAG) $(@:.o=.cpp)
cache_base.o: cache_base.h cache_base.cpp
	$(CC) $(PCFLAG) $(@:.o=.cpp)
main.o: main.cpp
	$(CC) $(PCFLAG) $(@:.o=.cpp)
$(TARGET): $(OBJ)
	$(CC) $(POFLAG) $(LIB) $(OBJ) -o $@

clean:
	rm $(OBJ)
	rm $(TARGET)

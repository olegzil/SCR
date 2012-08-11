# Makefile to build monte_pi_sprng program
# --- macros

cxxflags_default = -fast -ffast-math -fexpensive-optimizations -DNDEBUG
cxxflags_small   = -Os -ffast-math -DJSON_LESS_MEMORY -DNDEBUG
cxxflags_debug   = -g -DJSON_SAFE -DJSON_DEBUG


CC=g++-4.2
CFLAGS = -I/usr/include
CFLAGS +=  -g -I../CommonCode  
CFLAGS += -I/usr/local/include/boost_1_49_0
CFLAGS += $(cxxflags_default)


OBJECTS= main.o
#LIBS = -L/usr/local/lib/sprng/lib -llcg -L/usr/local/lib/pgplot -lcpgplot -lpgplot -lX11 -lftn -lm

# --- targets
all:    JsonTest
JsonTest:   $(OBJECTS) 
	$(CC) $(CFLAGS) -o JsonTest $(OBJECTS) 
        
main.o: main.cpp ../CommonCode/JsonParser.h
	$(CC) $(CFLAGS) -c main.cpp
       

# --- remove binary and executable files
clean:
	rm -f $(OBJECTS)

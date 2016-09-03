# which compiler
CC = gcc

# Include directory
INCLUDE = .

# Where to install
INSTDIR = .

# Libraries
LIBS =

# compiler options
CFLAGS =

# All the object files
OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))

all: codecounter

codecounter: $(OBJECTS)
	$(CC) -I$(INCLUDE) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $^ -o $@

.PHONY: clean

clean:
	-rm -rf *.o
	-rm -rf codecounter.exe codecounter

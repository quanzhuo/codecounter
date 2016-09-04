# which compiler
CC = gcc

# Include directory
INCLUDE = include

# Where to install
INSTDIR = .

# Libraries
LIBS =

# compiler options
CFLAGS = -g

# All the object files
OBJECTS := $(patsubst src%.c, src%.o, $(wildcard src/*.c))

all: codecounter

codecounter: $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $(OBJECTS) $(LIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $^ -o $@

.PHONY: clean

clean:
	-rm -rf src/*.o
	-rm -rf codecounter.exe codecounter

install: codecounter
	@if [ ! -d $INSTDIR ]; then\
		 mkdir $INSTDIR; \
	 fi; \
	 cp codecounter* $INSTDIR; \
	 echo "installed in ${INSTDIR}"


CC = gcc
LD = ld
MAKE = make
CP = cp
CD = cd
MV = mv
RM = -rm -fr

TARGET = libusbvb0.dll

OBJECTS = libusbvb0.o


VPATH = .

INCLUDES = -I../../src

CFLAGS = -O2 -Wall -mno-cygwin

LDFLAGS = -s -mwindows -shared -mno-cygwin \
					-Wl,--kill-at -Wl,--add-stdcall-alias \
					-L../.. -lusb	

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES) 

.PHONY: clean
clean:	
	$(RM) *.o *.dll *~


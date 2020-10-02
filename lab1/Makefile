CC = gcc       # compiler
CFLAGS = -Wall -g -std=c99 # compilation flg
LD = gcc       # linker
LDFLAGS = -g   # debugging symbols in build
LDLIBS = -lz   # link with libz

# For students
LIB_UTIL = zutil.o crc.o
SRCS   = findpng.c pnginfo.c catpng.c crc.c zutil.c
OBJS   = pnginfo.o $(LIB_UTIL)

TARGETS= catpng findpng pnginfo

all: ${TARGETS}

pnginfo: $(OBJS)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

findpng: findpng.o
	$(LD) -o $@ $^ $(LDLIBS)

catpng: catpng.o
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.d: %.c
	gcc -MM -MF $@ $<

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGETS)

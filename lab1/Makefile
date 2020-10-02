CC = gcc       # compiler
CFLAGS = -Wall -g -std=c99 # compilation flg
LD = gcc       # linker
LDFLAGS = -g   # debugging symbols in build
LDLIBS = -lz   # link with libz

# For students
LIB_UTIL = zutil.o crc.o
SRCS   = catpng.c crc.c zutil.c findpng.c pnginfo.c
OBJS   = catpng.o $(LIB_UTIL)

# TARGETS= catpng findpng pnginfo
TARGETS= findpng

all: ${TARGETS}

# catpng: $(OBJS)
# 	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

findpng: findpng.o
	$(LD) -o $@ $^ $(LDLIBS)

# pnginfo: pnginfo.o
# 	$(LD) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.d: %.c
	gcc -MM -MF $@ $<

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGETS)

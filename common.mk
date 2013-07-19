TARGET = mongo

SOURCES = bsonobj.cpp client.cpp
OBJECTS = $(SOURCES:.cpp=.o)

C_DRIVER_SRC = c_driver/src/bcon.c c_driver/src/bson.c \
							 c_driver/src/encoding.c c_driver/src/gridfs.c \
							 c_driver/src/md5.c c_driver/src/mongo.c \
							 c_driver/src/numbers.c c_driver/src/env.c

C_DRIVER_OBJ = $(C_DRIVER_SRC:.c=.o)

OBJECTS += $(C_DRIVER_OBJ)

MAJOR_VERSION = 0
SUB_VERSION = 1
PATCH_VERSION = 0

ALL_DEFINES=$(DEFINES)
ALL_DEFINES+=-D_POSIX_SOURCE -DMONGO_HAVE_STDINT

CC = gcc
CXX = g++
LD = gcc
INCPATH = -I.
RANLIB = ranlib
STRIP = strip
AR = ar rcu

PEDANTIC = -pedantic
OP_LEVEL ?= -O2
DEBUG = -gdwarf-4

STRICT = -Wextra -pedantic -Wformat=2 -Wshadow -Wpointer-arith
STRICT += -Wcast-qual -Wwrite-strings -Wstrict-overflow=4 
CPPFLAGS = -pipe -Wall $(STRICT) -Werror -std=c++0x -fPIC $(OP_LEVEL) $(DEBUG) $(ALL_DEFINES)
LFLAGS = -shared
CFLAGS = -std=c99 -fPIC $(PEDANTIC) $(OP_LEVEL) $(DEBUG) $(ALL_DEFINES)

# run `make dep` to get these dependencies (linux only)
c_driver/src/bcon.o: c_driver/src/bcon.c c_driver/src/bcon.h c_driver/src/bson.h
c_driver/src/bson.o: c_driver/src/bson.c c_driver/src/bson.h c_driver/src/encoding.h
c_driver/src/encoding.o: c_driver/src/encoding.c c_driver/src/bson.h \
 c_driver/src/encoding.h
c_driver/src/env.o: c_driver/src/env.c c_driver/src/env.h c_driver/src/mongo.h \
 c_driver/src/bson.h
c_driver/src/gridfs.o: c_driver/src/gridfs.c c_driver/src/gridfs.h \
 c_driver/src/mongo.h c_driver/src/bson.h
c_driver/src/md5.o: c_driver/src/md5.c c_driver/src/md5.h c_driver/src/bson.h
c_driver/src/mongo.o: c_driver/src/mongo.c c_driver/src/mongo.h c_driver/src/bson.h \
 c_driver/src/md5.h c_driver/src/env.h
c_driver/src/numbers.o: c_driver/src/numbers.c

dep:
	gcc -MM -DMONGO_HAVE_STDINT c_driver/src/*.c | sed 's#^\([^ ]\)#c_driver/src/\1#'


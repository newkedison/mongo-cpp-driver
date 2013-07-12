MONGO_OBJECTS=c_driver/src/bcon.o c_driver/src/bson.o c_driver/src/encoding.o \
							c_driver/src/gridfs.o c_driver/src/md5.o c_driver/src/mongo.o \
							c_driver/src/numbers.o c_driver/src/env.o
MONGO_C_LIB = c_driver/libmongoc.a c_driver/libbson.a

OBJECT = mongodriver
OBJECT_S = lib$(OBJECT).a

all: sub_c_driver $(OBJECT_S)

sub_c_driver:
	make -C c_driver

libmongodriver.a: bsonobj.o client.o $(MONGO_OBJECTS)
	ar rs $@ bsonobj.o client.o $(MONGO_OBJECTS)

%.o: %.cpp
	g++ -gdwarf-4 -Wall -Wextra -Werror -o $@ -DMONGO_HAVE_STDINT -c $< -std=c++11

%.o: %.c
	$(CC) -gdwarf-4 -o $@ -c -DMONGO_HAVE_STDINT $(ALL_CFLAGS) $<

test: FORCE
	make -C test

clean: FORCE
	make -C c_driver clean
	-rm *.o *.a

.PHONY: test FORCE


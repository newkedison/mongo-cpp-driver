first: all

include common.mk
DESTDIR = ..\..\lib
TARGET_S = lib$(TARGET).a
TARGET_D = $(TARGET).dll

all: $(DESTDIR)\$(TARGET_D) $(DESTDIR)\$(TARGET_S)

$(DESTDIR)\$(TARGET_D): $(OBJECTS) FORCE
	if not exist $(DESTDIR) mkdir $(DESTDIR)
	$(LD) $(LFLAGS) -o $(DESTDIR)$\\$(TARGET_D) $(OBJECTS) -lstdc++ -lws2_32

$(DESTDIR)\$(TARGET_S): $(OBJECTS) FORCE
	if not exist $(DESTDIR) mkdir $(DESTDIR)
	ar rcs $(DESTDIR)$\\$(TARGET_S)  $(OBJECTS)

%.o: %.cpp win32.mk
	$(CC) $(CPPFLAGS) $(INCPATH) -c $<

%.o: %.c win32.mk
	$(CC) -o $@ -c $(CFLAGS) $<

clean: FORCE
	-rm *.o

FORCE:

.PHONY: clean FORCE all


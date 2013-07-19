first: all

include common.mk
DESTDIR = ../../lib
ifdef TRAVIS
	DESTDIR = build
endif
TARGET_S = lib$(TARGET).a
TARGET_D = lib$(TARGET).so.$(MAJOR_VERSION).$(SUB_VERSION).$(PATCH_VERSION)
TARGET_D_1 = lib$(TARGET).so.$(MAJOR_VERSION).$(SUB_VERSION)
TARGET_D_2 = lib$(TARGET).so.$(MAJOR_VERSION)
TARGET_D_3 = lib$(TARGET).so
TARGET_D_SONAME = $(TARGET_D_2)

all: $(DESTDIR)/$(TARGET_D) $(DESTDIR)/$(TARGET_S)

$(DESTDIR)/$(TARGET_D): $(OBJECTS) FORCE
	@test -d $(DESTDIR) || mkdir $(DESTDIR)
	$(LD) $(LFLAGS) $(EXTRALIBS) -Wl,-soname,$(TARGET_D_SONAME) -o $(TARGET_D) $(OBJECTS)
	ln -s $(TARGET_D) $(TARGET_D_1)
	ln -s $(TARGET_D_1) $(TARGET_D_2)
	ln -s $(TARGET_D_2) $(TARGET_D_3)
	mv -f -t $(DESTDIR) $(TARGET_D) $(TARGET_D_1) $(TARGET_D_2) $(TARGET_D_3)

$(DESTDIR)/$(TARGET_S): $(OBJECTS) FORCE
	@test -d $(DESTDIR) || mkdir $(DESTDIR)
	$(AR) $@ $(OBJECTS)

%.o: %.cpp linux.mk
	$(CC) $(CPPFLAGS) $(INCPATH) -c $<

%.o: %.c linux.mk
	$(CC) -o $@ -c $(CFLAGS) $<

clean: FORCE
	-rm *.o

FORCE:

.PHONY: clean FORCE all

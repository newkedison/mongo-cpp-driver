PLATFORM = none
PLATFORMS = linux win32

first: $(PLATFORM)

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATFORMS)"

linux:
	$(MAKE) -f linux.mk

win32:
	$(MAKE) -f win32.mk

clean:
	-rm *.o

PLATFORM = none
PLATFORMS = linux win32

first: $(PLATFORM)

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATFORMS)"

linux: a.out

win32: test.exe

a.out: main.o
	g++ -o $@ $< -L../../../lib -Wl,-Bstatic -lmongo -Wl,-Bdynamic

test.exe: main.o
	g++ -o $@ $< -L../../../lib -Wl,-Bstatic -lmongo -Wl,-Bdynamic -lws2_32

%.o: %.cpp
	g++ -gdwarf-4 -Wall -Wextra -Werror -o $@ -DMONGO_HAVE_STDINT -c $< -std=c++11 -I../../../include


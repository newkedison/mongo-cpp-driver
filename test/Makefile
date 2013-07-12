all: a.out

a.out: main.o ../libmongodriver.a
	g++ -o $@ $< -L.. -lmongodriver

%.o: %.cpp
	g++ -gdwarf-4 -Wall -Wextra -Werror -o $@ -DMONGO_HAVE_STDINT -c $< -std=c++11

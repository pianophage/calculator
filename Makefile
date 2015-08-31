# Makefile.

progs = calculator
libs = -lreadline
flags = -std=gnu++14 -Wall -Wextra -Werror -g
cxx = g++

.PHONY: all
all: $(progs)

calculator: calculator.cc
	$(cxx) $(flags) -o $@ $< $(libs)

.PHONY: clean
clean:
	-rm -f $(progs) core

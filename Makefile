# Makefile.

prog = calculator
objs = calculator.o evaluator.o scanner.o
libs = -lreadline
flags = -std=gnu++14 -Wall -Wextra -Werror -g
cxx = g++

%.o: %.cc
	$(cxx) $(flags) -o $@ -c $<

%: %.o
	$(cxx) -o $@ $^ $(libs)

.PHONY: all
all: $(prog)

$(prog): $(objs)

calculator.o: calculator.cc
evaluator.o: evaluator.cc
scanner.o: scanner.cc

.PHONY: clean
clean:
	-rm -f $(prog) $(objs) core

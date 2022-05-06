
SRC = $(wildcard *.cc)
OBJS = $(subst .cc,.o, $(SRC))
EXEC = $(subst .cc,, $(SRC))
CXXFLAGS = -std=c++17
%.o: %.cc
	g++ $(CXXFLAGS) $^ -o $@

%: %.o
	g++ $^ -o $@

.PHONY: all clean

all: $(EXEC)

# all:
# 	g++ lesson1.cc -o lesson1
# 	g++ decltype.cc -o decltype

clean:
	rm $(EXEC)
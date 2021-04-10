INCS = cmdline.h expr.h parse.h val.h pointer.h env.h catch.h cont.h step.h

OBJS = cmdline.o expr.o main.o parse.o val.o env.o cont.o step.o

CXXFLAGS = --std=c++14 -O2 -Wall -fsanitize=undefined -fno-sanitize-recover=undefined


OTHERDIR = ../test_msdscript/test_msdscript

TEST_INCS = $(OTHERDIR)/exec.h $(OTHERDIR)/testgen.h

TEST_SRCS = $(OTHERDIR)/exec.cpp $(OTHERDIR)/testgen.cpp $(OTHERDIR)/test_msdscript.cpp

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

cmdline.o: $(INCS)
expr.o: $(INCS)
main.o: $(INCS)
parse.o: $(INCS)
val.o: $(INCS)
env.o: $(INCS)
step.o: $(INCS)
cont.o: $(INCS)

test_msdscript: $(TEST_SRCS) $(TEST_INCS)
	c++ --std=c++14 -O2 -o test_msdscript $(TEST_SRCS)
	
.PHONY: test
test: msdscript
		./msdscript --test

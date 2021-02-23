INCS = cmdline.h expr.h parse.h

SRCS = cmdline.cpp main.cpp expr.cpp parse.cpp

OTHERDIR = ../test_msdscript/test_msdscript

TEST_INCS = $(OTHERDIR)/exec.h $(OTHERDIR)/testgen.h

TEST_SRCS = $(OTHERDIR)/exec.cpp $(OTHERDIR)/testgen.cpp $(OTHERDIR)/test_msdscript.cpp

msdscript: $(SRCS) $(INCS)
	c++ --std=c++14 -O2 -o msdscript $(SRCS)

test_msdscript: $(TEST_SRCS) $(TEST_INCS)
	c++ --std=c++14 -O2 -o test_msdscript $(TEST_SRCS)
	
.PHONY: test
test: msdscript
		./msdscript --test

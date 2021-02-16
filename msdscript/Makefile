INCS = cmdline.h expr.h parse.h

SRCS = cmdline.cpp main.cpp expr.cpp parse.cpp

msdscript: $(SRCS) $(INCS)
	c++ --std=c++14 -O2 -o msdscript $(SRCS)
	
.PHONY: test
test: msdscript
		./msdscript --test

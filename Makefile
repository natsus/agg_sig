CXX = gcc
LIBS = -ltepla -lgmp
CXXFLAGS  = -Wall -g -O4

TARGET = main

all: $(TARGET)

%: %.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

%.o: agg_sig.h

.PHONY: clean
clean:
	$(RM) *~ $(TARGET)

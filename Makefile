CXX = g++
LIBS = -ltepla -lcrypto -lgmp
CXXFLAGS  = -Wall -g -O4

TARGET = main speed_check speed_check2
OBJ = main.o speed_check.o speed_check2.o
LIBDIR = -L/usr/local/lib
INCDIR = -I/usr/local/include

all: $(TARGET)

%: %.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(INCDIR) $(LIBDIR) $(LIBS)
 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCDIR)

$(OBJ): agg_sig.h

.PHONY: clean
clean:
	$(RM) *~ $(TARGET) $(OBJ)

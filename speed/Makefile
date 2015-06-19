CXX = g++
LIBS = -ltepla -lcrypto -lgmp
CXXFLAGS  = -Wall -g -O4

TARGET = speed_check_gdh speed_check_agg
OBJ = speed_check_gdh.o speed_check_agg.o
LIBDIR = -L/usr/local/lib
INCDIR = -I/usr/local/include

all: $(TARGET)

%: %.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(INCDIR) $(LIBDIR) $(LIBS)
 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCDIR)

$(OBJ): ../agg_sig.h

.PHONY: clean
clean:
	$(RM) *~ $(TARGET) $(OBJ)

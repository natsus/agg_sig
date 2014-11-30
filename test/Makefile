CXX = g++
LIBS = -ltepla -lcrypto -lgmp
CXXFLAGS = -Wall -g -O4

TARGET = test.exe
OBJ = test.o
LIBDIR = -L/usr/local/lib
INCDIR = -I/usr/local/include

all: $(TARGET)

%.exe: %.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(INCDIR) $(LIBDIR) $(LIBS)
 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCDIR)

.PHONY: clean
clean:
	$(RM) *~ $(TARGET) $(OBJ)

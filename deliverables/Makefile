CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra
LDLIBS = -pthread

TARGET = mt-collatz

.PHONY: all clean

all: $(TARGET)

$(TARGET): mt-collatz.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET)

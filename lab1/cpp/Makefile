# Linux:   make
# Windows: mingw32-make   (получится dbms.exe)
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SOURCES  = src/main.cpp src/commands.cpp src/array.cpp src/flist.cpp \
           src/dlist.cpp src/stack.cpp src/queue.cpp src/fbt.cpp

ifeq ($(OS),Windows_NT)
    TARGET = dbms.exe
    RM     = del /Q
else
    TARGET = dbms
    RM     = rm -f
endif

$(TARGET): $(SOURCES) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	$(RM) $(TARGET)

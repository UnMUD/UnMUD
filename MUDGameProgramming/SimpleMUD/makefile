# Compiler is G++
CXX = g++

#MUDLib directory
MUDLIBDIR = ../Libraries
SOCKETLIBDIR = $(MUDLIBDIR)/SocketLib
THREADLIBDIR = $(MUDLIBDIR)/ThreadLib
BASICLIBDIR  = $(MUDLIBDIR)/BasicLib
SIMPLEMUDDIR = ./SimpleMUD

# libraries
LIBS = -lpthread

# flags
CFLAGS = -I$(MUDLIBDIR)

libs: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) $(SOCKETLIBDIR)/*.cpp -c;
	$(CXX) $(CFLAGS) $(THREADLIBDIR)/*.cpp -c;
	$(CXX) $(CFLAGS) $(BASICLIBDIR)/*.cpp -c;

simplemud: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) *.cpp -c;
	$(CXX) $(CFLAGS) $(SIMPLEMUDDIR)/*.cpp -c;

main: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) *.cpp -c;

link: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) *.o $(LIBS)

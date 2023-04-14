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
CFLAGS = -std=c++2a -I$(MUDLIBDIR)

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
	$(CXX) $(CFLAGS) *.o $(LIBS) -o UnMUD.out

run:
	./UnMUD.out

clean:
	rm -rf *.o
	rm UnMUD.out

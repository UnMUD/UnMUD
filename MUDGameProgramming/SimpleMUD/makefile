# Compiler is G++
CXX = g++

#MUDLib directory
MUDLIBDIR = ../Libraries
SOCKETLIBDIR = $(MUDLIBDIR)/SocketLib
THREADLIBDIR = $(MUDLIBDIR)/ThreadLib
BASICLIBDIR  = $(MUDLIBDIR)/BasicLib
TESTSDIR = ../Tests
TESTSMAINFILE = Tests
TESTSBINFILE = TestsUnMUD.out
UNMUDDIR = ./SimpleMUD
UNMUDMAINFILE = SimpleMUD
UNMUDBINFILE = UnMUD.out

# libraries
LIBS = -lpthread
TESTSLIBS = -lgtest

# flags
CFLAGS = -std=c++2a -I$(MUDLIBDIR)

libs: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) $(SOCKETLIBDIR)/*.cpp -c --coverage;
	$(CXX) $(CFLAGS) $(THREADLIBDIR)/*.cpp -c --coverage;
	$(CXX) $(CFLAGS) $(BASICLIBDIR)/*.cpp -c --coverage;

unmud: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) *.cpp -c --coverage;
	$(CXX) $(CFLAGS) $(UNMUDDIR)/*.cpp -c --coverage;

main: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) *.cpp -c;

link: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) \
	$$(find . -maxdepth 1 -name '*.o' ! -name '$(TESTSMAINFILE).o' -exec basename {} \;) \
	$(LIBS) -o $(UNMUDBINFILE) --coverage

run:
	./$(UNMUDBINFILE)

tests: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) $(TESTSDIR)/*.cpp -c --coverage;
	$(CXX) $(CFLAGS) \
	$$(find . -maxdepth 1 -name '*.o' ! -name '$(UNMUDMAINFILE).o' -exec basename {} \;) \
	$(TESTSLIBS) $(LIBS) -o $(TESTSBINFILE) --coverage

run_tests:
	rm *.gcda
	./$(TESTSBINFILE)

clean:
	rm -rf *.o
	rm $(UNMUDBINFILE)
	rm $(TESTSBINFILE)

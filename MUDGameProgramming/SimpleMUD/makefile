# Compiler is G++
CXX = g++

#MUDLib directory
MUDLIBDIR = ../Libraries
SOCKETLIBDIR = $(MUDLIBDIR)/SocketLib
THREADLIBDIR = $(MUDLIBDIR)/ThreadLib
BASICLIBDIR  = $(MUDLIBDIR)/BasicLib
TESTSDIR = ../Tests
TESTFILEPREFIX = Test
TESTSMAINFILE = Tests
TESTSBINFILE = TestsUnMUD.out
UNMUDDIR = ./SimpleMUD
UNMUDMAINFILE = SimpleMUD
UNMUDBINFILE = UnMUD.out

# libraries
LIBS = -lpthread -lpqxx
TESTSLIBS = -lgtest

# flags
CFLAGS = -std=c++2a -I$(MUDLIBDIR) # \
		-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast \
		-Wcast-align -Wunused -Woverloaded-virtual -Wconversion \
		-Wsign-conversion -Wmisleading-indentation -Weffc++ -pedantic

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
	$$(find . -maxdepth 1 -name '*.o' ! -name '$(TESTSMAINFILE).o' \
	! -name '$(TESTFILEPREFIX)*' -exec basename {} \;) \
	$(LIBS) -o $(UNMUDBINFILE) --coverage

run:
	./$(UNMUDBINFILE)

tests: $(wildcard *.cpp)
	$(CXX) $(CFLAGS) $(TESTSDIR)/*.cpp -c --coverage;
	$(CXX) $(CFLAGS) \
	$$(find . -maxdepth 1 -name '*.o' ! -name '$(UNMUDMAINFILE).o' -exec basename {} \;) \
	$(TESTSLIBS) $(LIBS) -o $(TESTSBINFILE) --coverage

run_tests:
	rm -f *.gcda
	./$(TESTSBINFILE)

clean:
	rm -f *.o *.gcda *.gcno
	rm -f $(UNMUDBINFILE)
	rm -f $(TESTSBINFILE)

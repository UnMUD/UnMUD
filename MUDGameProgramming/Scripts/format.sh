MUDLIBDIR=./Libraries
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD

if [ "$1" == "-l" ]; then
    clang-format --style=llvm -i $SOCKETLIBDIR/*.cpp $SOCKETLIBDIR/*.h
    clang-format --style=llvm -i $THREADLIBDIR/*.cpp $THREADLIBDIR/*.h
    clang-format --style=llvm -i $BASICLIBDIR/*.cpp $BASICLIBDIR/*.h
elif [ "$1" == "-m" ]; then
    clang-format --style=llvm -i $SIMPLEMUD/*.cpp $SIMPLEMUD/*.h $SIMPLEMUD/**/*.cpp $SIMPLEMUD/**/*.h
else
    echo "Use the flag -l to test Libraries or -m to test the MUD"
fi

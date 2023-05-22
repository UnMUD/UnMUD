MUDLIBDIR=./Libraries
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD

if [ "$1" == "-s" ]; then
    clang-format --style=llvm -i $SOCKETLIBDIR/*.cpp $SOCKETLIBDIR/*.h
elif [ "$1" == "-t" ]; then
    clang-format --style=llvm -i $THREADLIBDIR/*.cpp $THREADLIBDIR/*.h
elif [ "$1" == "-b" ]; then
    clang-format --style=llvm -i $BASICLIBDIR/*.cpp $BASICLIBDIR/*.h
elif [ "$1" == "-m" ]; then
    clang-format --style=llvm -i $SIMPLEMUD/*.cpp $SIMPLEMUD/**/*.cpp $SIMPLEMUD/**/*.h
else
    echo "Use the flag -m to test the MUD, -b to test the BasicLib, -s to test the SocketLib or -t to test the ThreadLib"
fi

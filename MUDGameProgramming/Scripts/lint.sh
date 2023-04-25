MUDLIBDIR=./Libraries
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD
DATE=$(date +"%Y-%m-%d-%H-%M-%S")

if [ "$1" == "-s" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$SOCKETLIB/$DATE-clang-tidy.yml \
    $SOCKETLIBDIR/*.cpp $SOCKETLIBDIR/*.h -- -std=c++2a -I$MUDLIBDIR
elif [ "$1" == "-t" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$THREADLIB/$DATE-clang-tidy.yml \
    $THREADLIBDIR/*.cpp $THREADLIBDIR/*.h -- -std=c++2a -I$MUDLIBDIR
elif [ "$1" == "-b" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$BASICLIB/$DATE-clang-tidy.yml \
    $BASICLIBDIR/*.cpp $BASICLIBDIR/*.h -- -std=c++2a -I$MUDLIBDIR
elif [ "$1" == "-m" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$SIMPLEMUD/$DATE-clang-tidy.yml \
    $SIMPLEMUD/*.cpp $SIMPLEMUD/*.h $SIMPLEMUD/**/*.cpp $SIMPLEMUD/**/*.h -- -std=c++2a -I$MUDLIBDIR
else
    echo "Use the flag -m to test the MUD, -b to test the BasicLib, -s to test the SocketLib or -t to test the ThreadLib"
fi

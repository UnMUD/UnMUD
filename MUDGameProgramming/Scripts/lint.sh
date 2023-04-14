MUDLIBDIR=./Libraries
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD

if [ "$1" == "-l" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$SOCKETLIB/clang-tidy.yml \
    $SOCKETLIBDIR/*.cpp $SOCKETLIBDIR/*.h -- -I$MUDLIBDIR
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$THREADLIB/clang-tidy.yml \
    $THREADLIBDIR/*.cpp $THREADLIBDIR/*.h -- -I$MUDLIBDIR
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$BASICLIB/clang-tidy.yml \
    $BASICLIBDIR/*.cpp $BASICLIBDIR/*.h -- -I$MUDLIBDIR
elif [ "$1" == "-m" ]; then
    clang-tidy -checks=* --format-style='llvm' --export-fixes=./AnalysisLogs/$SIMPLEMUD/clang-tidy.yml \
    $SIMPLEMUD/*.cpp $SIMPLEMUD/*.h $SIMPLEMUD/**/*.cpp $SIMPLEMUD/**/*.h -- -I$MUDLIBDIR
else
    echo "Use the flag -l to test Libraries or -m to test the MUD"
fi

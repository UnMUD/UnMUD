MUDLIBDIR=./Libraries
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD

if [ "$1" == "-l" ]; then
    cppcheck  --cppcheck-build-dir=./AnalysisLogs/$SOCKETLIB/ --template=gcc --enable=all $SOCKETLIBDIR
    cppcheck  --cppcheck-build-dir=./AnalysisLogs/$THREADLIB/ --template=gcc --enable=all $THREADLIBDIR
    cppcheck  --cppcheck-build-dir=./AnalysisLogs/$BASICLIB/ --template=gcc --enable=all $BASICLIBDIR
elif [ "$1" == "-m" ]; then
    cppcheck  --cppcheck-build-dir=./AnalysisLogs/$SIMPLEMUD --template=gcc --enable=all ./$SIMPLEMUD
else
    echo "Use the flag -l to test Libraries or -m to test the MUD"
fi
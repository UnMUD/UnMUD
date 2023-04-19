MUDLIBDIR=./Libraries
ANALYSISDIR=./AnalysisLogs
SOCKETLIB=SocketLib
THREADLIB=ThreadLib
BASICLIB=BasicLib
SOCKETLIBDIR=$MUDLIBDIR/$SOCKETLIB
THREADLIBDIR=$MUDLIBDIR/$THREADLIB
BASICLIBDIR=$MUDLIBDIR/$BASICLIB
SIMPLEMUD=SimpleMUD
DATE=$(date +"%Y-%m-%d-%H-%M-%S")

if [ "$1" == "-l" ]; then
    mkdir $ANALYSISDIR/$SOCKETLIB/$DATE
    cppcheck  --cppcheck-build-dir=$ANALYSISDIR/$SOCKETLIB/$DATE --template=gcc --enable=all $SOCKETLIBDIR
    mkdir $ANALYSISDIR/$THREADLIB/$DATE
    cppcheck  --cppcheck-build-dir=$ANALYSISDIR/$THREADLIB/$DATE --template=gcc --enable=all $THREADLIBDIR
    mkdir $ANALYSISDIR/$BASICLIB/$DATE
    cppcheck  --cppcheck-build-dir=$ANALYSISDIR/$BASICLIB/$DATE --template=gcc --enable=all $BASICLIBDIR
elif [ "$1" == "-m" ]; then
    mkdir $ANALYSISDIR/$SIMPLEMUD/$DATE
    cppcheck  --cppcheck-build-dir=$ANALYSISDIR/$SIMPLEMUD/$DATE --template=gcc --enable=all ./$SIMPLEMUD
else
    echo "Use the flag -l to test Libraries or -m to test the MUD"
fi
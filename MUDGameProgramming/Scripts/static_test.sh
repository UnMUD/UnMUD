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

if [ "$1" == "-s" ]; then
    cppcheck --xml --cppcheck-build-dir=$ANALYSISDIR/$SOCKETLIB/cppcheckbuild --std=c++20 --template=gcc \
    --enable=all --error-exitcode=1 $SOCKETLIBDIR 2>&1 | tee $ANALYSISDIR/$SOCKETLIB/$DATE-cppcheck.xml
    exit ${PIPESTATUS[0]}
elif [ "$1" == "-t" ]; then 
    cppcheck --xml --cppcheck-build-dir=$ANALYSISDIR/$THREADLIB/cppcheckbuild --std=c++20 --template=gcc \
    --enable=all --error-exitcode=1 $THREADLIBDIR 2>&1 | tee $ANALYSISDIR/$THREADLIB/$DATE-cppcheck.xml
    exit ${PIPESTATUS[0]}
elif [ "$1" == "-b" ]; then
    cppcheck --xml --cppcheck-build-dir=$ANALYSISDIR/$BASICLIB/cppcheckbuild --std=c++20 --template=gcc \
    --enable=all --error-exitcode=1 $BASICLIBDIR 2>&1 | tee $ANALYSISDIR/$BASICLIB/$DATE-cppcheck.xml
    exit ${PIPESTATUS[0]}
elif [ "$1" == "-m" ]; then
    cppcheck --xml --cppcheck-build-dir=$ANALYSISDIR/$SIMPLEMUD/cppcheckbuild --std=c++20 --template=gcc \
    --enable=all --error-exitcode=1 ./$SIMPLEMUD 2>&1 | tee $ANALYSISDIR/$SIMPLEMUD/$DATE-cppcheck.xml
    exit ${PIPESTATUS[0]}
else
    echo "Use the flag -m to test the MUD, -b to test the BasicLib, -s to test the SocketLib or -t to test the ThreadLib"
fi
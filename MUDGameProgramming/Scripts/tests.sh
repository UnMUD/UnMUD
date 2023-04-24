COVERAGEDIR=../AnalysisLogs/GCOVR
DATE=$(date +"%Y-%m-%d-%H-%M-%S")

cd SimpleMUD 

if make tests ; then
    echo "Tests compiled"
else
    errorNum=$?
    echo "Error $errorNum compiling Tests..."
    exit $errorNum
fi

if make run_tests ; then
    gcovr &> $COVERAGEDIR/coverage.txt
    cp $COVERAGEDIR/coverage.txt $COVERAGEDIR/$DATE-coverage.txt
else
    errorNum=$?
    echo "Error $errorNum running tests"
    exit $errorNum
fi

COVERAGEDIR=../AnalysisLogs/GCOVR
DATE=$(date +"%Y-%m-%d-%H-%M-%S")

cd SimpleMUD 

if make tests ; then
    echo "Tests compiled"
else
    echo "Error $? compiling Tests..."
    exit
fi

if make run_tests ; then
    gcovr &> $COVERAGEDIR/coverage.txt
    cp $COVERAGEDIR/coverage.txt $COVERAGEDIR/$DATE-coverage.txt
else
    echo "Error $? running tests"
    exit
fi

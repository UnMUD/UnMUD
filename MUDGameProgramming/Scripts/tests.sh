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
    gcovr --fail-under-line 80 -r ../ -e ../Tests/*.cpp -e ../Tests/*.h  \
    --html $COVERAGEDIR/$DATE-coverage.html --sonarqube $COVERAGEDIR/coverage.xml
else
    errorNum=$?
    echo "Error $errorNum running tests"
    exit $errorNum
fi

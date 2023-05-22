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

make run_tests

testsRet=$?

gcovr --fail-under-line 80 -r ../ -e ../Tests/  \
    --html $COVERAGEDIR/$DATE-coverage.html --sonarqube $COVERAGEDIR/coverage.xml

echo "Running tests finished with return $testsRet"
exit $errorNum

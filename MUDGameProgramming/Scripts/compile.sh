cd SimpleMUD 

if make libs ; then
    echo "Libs compiled"
else
    errorNum=$?
    echo "Error $errorNum compiling Libs..."
    exit $errorNum
fi

if make unmud ; then
    echo "UnMUD compiled"
else
    errorNum=$?
    echo "Error $errorNum compiling UnMUD..."
    exit $errorNum
fi

if make link ; then
    echo "Objects linked, binary file created"
else
    errorNum=$?
    echo "Error $errorNum linking objects..."
    exit $errorNum
fi

exit
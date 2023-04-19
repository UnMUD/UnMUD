cd SimpleMUD 

if make libs ; then
    echo "Libs compiled"
else
    echo "Error $? compiling Libs..."
    exit
fi

if make unmud ; then
    echo "UnMUD compiled"
else
    echo "Error $? compiling UnMUD..."
    exit
fi

if make link ; then
    echo "Objects linked, binary file created"
else
    echo "Error $? linking objects..."
    exit
fi

exit
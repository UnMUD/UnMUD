cd SimpleMUD 

if make libs ; then
    echo "Libs compiled"
else
    echo "Error $? compiling Libs..."
    exit
fi

if make simplemud ; then
    echo "SimpleMUD compiled"
else
    echo "Error $? compiling SimpleMUD..."
    exit
fi

if make link ; then
    echo "Objects linked, binary file created"
else
    echo "Error $? linking objects..."
    exit
fi

exit
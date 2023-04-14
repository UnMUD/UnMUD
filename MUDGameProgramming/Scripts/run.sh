cd SimpleMUD

FILE=UnMUD.out
if [ -f "$FILE" ]; then
    echo "$FILE found. Executing binary..."
    make run
else 
    echo "$FILE does not exist."
fi
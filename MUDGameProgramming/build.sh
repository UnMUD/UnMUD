# set -euxo pipefail

cd SimpleMUD && make libs && make simplemud && make link

echo $?

if [ $? -eq 0 ]; then
    echo "MUD compiled and linked"
    echo "Running UnMUD" && ./a.out
else
    echo "Something went wrong..."
fi
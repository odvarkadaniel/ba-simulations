#!/bin/sh

OPTIND=1

input_file=""
simulation=""
delayed=0
fair=0
direct=0

while getopts "s:f:" opt; do
    case "$opt" in
    f) input_file=$OPTARG
        ;;
    s) simulation=$OPTARG
        ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    esac
done

if [ "$simulation" = "fair" ]
then
    fair=1
elif [ "$simulation" = "direct" ]
then
    direct=1
elif [ "$simulation" = "delayed" ]
then
    delayed=1
else
    echo "Supported simulation types are fair, direct and delayed."
    exit 1
fi

if [ $fair = 1 ]
then
    ./main --file=$input_file --fair --dot && dot -Tpng result.dot > output.png
elif [ $delayed = 1 ]
then
    ./main --file=$input_file --delayed --dot && dot -Tpng result.dot > output.png
else
    ./main --file=$input_file --direct --dot && dot -Tpng result.dot > output.png
fi

echo "Dot file named result.dot was created..."
echo "Dot file was converted to output.png..."



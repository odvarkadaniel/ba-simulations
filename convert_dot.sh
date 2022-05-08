#!/bin/sh

OPTIND=1

input_file=""
simulation=""
delayed=0
fair=0
direct=0

while getopts "s:f:h" opt; do
    case "$opt" in
    f) input_file=$OPTARG
        ;;
    s) simulation=$OPTARG
        ;;
    h)
        printf "Examples of usage:\n"
        printf "  ./nba-simulations -f test_file.ba -s fair\n"
        printf "  ./nba-simulations -f test_file.ba -s delayed\n"
        printf "\n\n"
        printf "Usage:\n"
        printf "  ./nba-simulations { -f -s }\n\n"
        printf "Available options:\n"
        printf "  -f    The input file in format .ba\n"
        printf "  -s    The type of simulation to compute { fair, direct, delayed }\n"
        exit 0
        ;;
    -*|--*)
        printf "Unknown option $1"
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
    printf "Supported simulation types are fair, direct and delayed."
    exit 1
fi

if [ $fair = 1 ]
then
    ./main --file=$input_file --fair --dot && dot -Tpng result.dot > output.png
elif [ $delayed = 1 ]
then
    ./main --file=$input_file --delayed --fast --dot && dot -Tpng result.dot > output.png
else
    ./main --file=$input_file --direct --fast --dot && dot -Tpng result.dot > output.png
fi

printf "Dot file named result.dot was created..."
printf "Dot file was converted to output.png..."



#!/bin/bash
#
# Telefon (checker)
#

#
# Base directory.
#
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

#
# Cheap hack used to fix double whitespaces not printing.
#
IFS='%'

#
# Array containing the name of every required rule.
#
RULES=(build run clean)

#
# Points assigned for each task.
#
MAXIMUM_SCORE=140
POINTS_MAKEFILE=5
POINTS_README=5

#
# Checking if `Makefile` exists.
#
if [[ ! -f Makefile ]]; then
    echo "ERROR: \`Makefile\` does not exist!"
    exit 1
fi

#
# Checking if all required rules are defined.
#
for rule in "${RULES[@]}"; do
    if ! grep -q -E "^$rule:" Makefile; then
        echo "ERROR: Rule \`$rule\` does not exist in \`Makefile\`."
        exit 2
    fi
done

#
# Checking if -Wall is used in compiling.
#
if ! grep -q -i -- "-Wall" Makefile; then
    echo "WARN: -Wall is not used as a parameter for gcc."
fi

#
# Making sure the latest version of source is built.
# We clean the old binaries.
#
if ! make clean &>/dev/null; then
    echo "ERROR: Cleaning failed!"
    exit 3
fi

#
# Rebuilding the source.
#
echo "INFO: Building...."
if ! make &>/dev/null; then
    echo "ERROR: Building failed!"
    exit 4
fi

#
# Checking if `telefon` exists.
#
if [[ ! -f telefon ]]; then
    echo "ERROR: \`telefon\` does not exist!"
    exit 5
fi

#
# Running the tests.
#

# Number of tests
TEST_N=22
# How many points / task
TASK_POINTS=(25 25 30 5 5 10 15 15)
# Name of task
TASK_NAME=("1" "2" "3" "4.1" "4.2" "4.3" "4.4" "4.5")
# Length of task
TASK_LEN=${#TASK_POINTS[@]}
# Total points
total=0

echo "INFO: Running tests..."

for ((i=0; i<TEST_N; ++i)); do
    test_name=test$i

    # Running the solution.
    # Timeout is used to make sure the process ends.
    if which timeout &>/dev/null; then
        timeout 3 ./telefon < "$DIR/tests/$test_name.in" &> "$test_name.out"
    else
        ./telefon < "$DIR/tests/$test_name.in" &> "$test_name.out"
    fi

    test_total=0
    for ((j=0; j<$TASK_LEN; ++j)); do
        LINE=$((j+1))

        # Getting the correct / computed output
        expected=$(sed "${LINE}q;d" "$DIR/tests/$test_name.out")
        actual=$(sed "${LINE}q;d" "$test_name.out")

        # Trimming whitespaces
        expected=$(echo -e "${expected}" | sed -e 's/[[:space:]]*$//')
        actual=$(echo -e "${actual}" | sed -e 's/[[:space:]]*$//')

        task_total=${TASK_POINTS[j]}
        if [[ "$expected" == "$actual" ]]; then
            test_total=$((test_total+task_total))
        else
            task_total=0
        fi
        printf "INFO: %-16s %-16s ... %3d / %3d\n" "Test $test_name.in" "Task ${TASK_NAME[j]}" $task_total ${TASK_POINTS[j]}

    done
    printf "%-36s TOTAL: %3d / %3d\n" " " $test_total "130"
    total=$((total+test_total))
    echo "======================================================================="

    #
    # Cleaning up
    #
    rm -f $test_name.out
done

total=$((total/TEST_N))
#
# Adding points for `Makefile`.
#
total=$(($total+$POINTS_MAKEFILE))
printf "INFO: %-33s ... %3d / %3d\n" "Makefile" $POINTS_MAKEFILE $POINTS_MAKEFILE

#
# Adding points for read me files (if any).
#
readme_file=$(ls | grep -i -E "read[_]?me" | head -n 1)
if [[ -n $readme_file ]]; then
    if [[ -s $readme_file ]]; then
        printf "INFO: %-33s ... %3d / %3d\n" "Read me ($readme_file)" $POINTS_README $POINTS_README
        total=$(($total+$POINTS_README))
    else
        printf "WARN: %-33s ... %3d / %3d\n" "Read me ($readme_file is empty)" 0 $POINTS_README
    fi
else
    printf "WARN: %-33s ... %3d / %3d\n" "Read me not found" 0 $POINTS_README
fi

#
# Running Cppcheck.
#
if which cppcheck &>/dev/null; then
    if [[ $(cppcheck telefon.c 2>&1 | wc -l) -le 1 ]]; then
        printf "INFO: %-33s ... Excellent!\n" "Cppcheck"
    else
        printf "WARN: %-33s ... Errors were found.\n" "Cppcheck"
    fi
fi

#
# Running Valgrind.
#
if which valgrind &>/dev/null; then
    if valgrind --tool=memcheck --leak-check=full ./telefon < $DIR/tests/test0.in 2>&1 | grep -i -E "All heap blocks were freed|definitely lost: 0 bytes" &>/dev/null; then
        printf "INFO: %-33s ... Excellent!\n" "Valgrind"
    else
        printf "WARN: %-33s ... Not all memory was freed.\n" "Valgrind"
    fi
fi

#
# Printing final score.
#
echo "======================================================================="
printf "INFO: %-33s ... %3d / %3d\n" "TOTAL" $total $MAXIMUM_SCORE
echo
echo "INFO: Punctajul pentru coding style (10p) va fi acordat dupa verificarea"
echo "      temei de catre unul din responsabilii pentru tema."

#
# Exiting...
#
make clean &>/dev/null
exit $total

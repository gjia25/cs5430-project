#!/bin/bash

total=3
passed=0

make clean
make

test_is_equal () {
    if cmp -s "$1" "$2"; then
        echo "TEST $1: PASSED"
    else
        echo "TEST $1: FAILED"
    fi
}
input="tests/parsing.txt"
output="tests/parsing_out.txt"
expected="tests/parsing_exp.txt"
./analyzer "$input" "$output"
test_is_equal "$expected" "$output"

input="tests/single_s.txt"
output="tests/single_s_out.txt"
expected="tests/single_s_exp.txt"
./analyzer "$input" "$output"
test_is_equal "$expected" "$output"

input="tests/multi_s.txt"
output="tests/multi_s_out.txt"
expected="tests/multi_s_exp.txt"
./analyzer "$input" "$output"
test_is_equal "$expected" "$output"

# for ((i=1;i<=total;i++));
# do
#     input="tests/in_$i.txt"
#     expected="tests/exp_$i.txt"
#     output="tests/out_$i.txt"

#     ./analyzer $input $output

#     if cmp -s "$expected" "$output"; then
#         let passed+=1
#         echo "TEST $i: PASSED"
#     else
#         echo "TEST $i: FAILED"
#     fi
# done

# echo "TESTS PASSED: $passed/$total"
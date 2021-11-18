#!/bin/bash

total=2
passed=0

make all

for ((i=1;i<=total;i++));
do
    input="tests/in_$i.txt"
    expected="tests/exp_$i.txt"
    output="tests/out_$i.txt"

    ./analyzer $input $output

    if cmp -s "$expected" "$output"; then
        let passed+=1
        echo "TEST $i: PASSED"
    else
        echo "TEST $i: FAILED"
    fi
done

echo "TESTS PASSED: $passed/$total"
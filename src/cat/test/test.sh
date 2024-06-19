#!/bin/bash

attempts=0
successes=0
failes=0

flags=("b" "e" "n" "s" "t" "v" "E")

test_func() {
    ../s21_cat "-$@" chars.txt smt.txt smt.txt > s21_cat_output
    cat "-$1" chars.txt smt.txt smt.txt > cat_output
    ((attempts++))
    if diff "s21_cat_output" "cat_output"; then
        ((successes++))
        echo "success result $successes with "-$1" flags at $attempts attempt"
    else
        ((failes++))
        echo "fail result - $failes with (-$1) flags at $attempts attempt"
    fi

}

# 1 param
for param1 in ${flags[@]}; do
    test_func $param1
done

#2 params
for param1 in ${flags[@]}; do
    for param2 in ${flags[@]}; do
    if [ $param1 != $param2 ]; then
        params="${param1}${param2}"
        test_func "$param1$param2"
    fi
    done
done

# 3 params
for param1 in ${flags[@]}; do
    for param2 in ${flags[@]}; do
        for param3 in ${flags[@]}; do
            if [ $param1 != $param2 ] && [ $param2 != $param3 ] && [ $param1 != $param3 ]; then
            test_func "$param1$param2$param3"
            fi
        done
    done
done

echo "Successes - $successes, Failes - $failes, Attempts - $attempts"